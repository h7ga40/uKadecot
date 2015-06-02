/*------------------------------------------------------------------------/
/  The Main Development Bench of FatFs Module
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2014, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/


#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <locale.h>
#include "diskio.h"
#include "ff.h"
#include "zlib.h"

int assign_drives(void);


#ifdef UNICODE
#if !_LFN_UNICODE
#error Configuration mismatch. _LFN_UNICODE must be 1.
#endif
#else
#if _LFN_UNICODE
#error Configuration mismatch. _LFN_UNICODE must be 0.
#endif
#endif



#if _MULTI_PARTITION	/* Volume - partition resolution table (Example) */
PARTITION VolToPart[] = {
	{ 0, 0 },	/* "0:" <== Disk# 0, auto detect */
	{ 1, 0 },	/* "1:" <== Disk# 1, auto detect */
	{ 2, 0 },	/* "2:" <== Disk# 2, auto detect */
	{ 3, 1 },	/* "3:" <== Disk# 3, 1st partition */
	{ 3, 2 },	/* "4:" <== Disk# 3, 2nd partition */
	{ 3, 3 },	/* "5:" <== Disk# 3, 3rd partition */
	{ 4, 0 },	/* "6:" <== Disk# 4, auto detect */
	{ 5, 0 }	/* "7:" <== Disk# 5, auto detect */
};
#endif


/*---------------------------------------------------------*/
/* Work Area                                               */
/*---------------------------------------------------------*/


LONGLONG AccSize;			/* Work register for scan_files() */
WORD AccFiles, AccDirs;
FILINFO Finfo;
#if _USE_LFN
TCHAR LFName[256];
#endif

TCHAR Line[300];			/* Console input/output buffer */
HANDLE hCon, hKey;

FATFS FatFs[_VOLUMES];		/* File system object for logical drive */
BYTE InBuff[262144];		/* Working buffer */
BYTE OutBuff[262144];		/* Working buffer */

#if _USE_FASTSEEK
DWORD SeekTbl[16];			/* Link map table for fast seek feature */
#endif


/*---------------------------------------------------------*/
/* User Provided RTC Function for FatFs module             */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support an RTC.                     */
/* This function is not required in read-only cfg.         */

DWORD get_fattime(void)
{
	SYSTEMTIME tm;

	/* Get local time */
	GetLocalTime(&tm);

	/* Pack date and time into a DWORD variable */
	return 	  ((DWORD)(tm.wYear - 1980) << 25)
		| ((DWORD)tm.wMonth << 21)
		| ((DWORD)tm.wDay << 16)
		| (WORD)(tm.wHour << 11)
		| (WORD)(tm.wMinute << 5)
		| (WORD)(tm.wSecond >> 1);
}



/*--------------------------------------------------------------------------*/
/* Monitor                                                                  */

/*----------------------------------------------*/
/* Get a value of the string                    */
/*----------------------------------------------*/
/*	"123 -5   0x3ff 0b1111 0377  w "
^                           1st call returns 123 and next ptr
^                        2nd call returns -5 and next ptr
^                3rd call returns 1023 and next ptr
^         4th call returns 15 and next ptr
^    5th call returns 255 and next ptr
^ 6th call fails and returns 0
*/

int xatoi(			/* 0:Failed, 1:Successful */
	TCHAR **str,	/* Pointer to pointer to the string */
	long *res		/* Pointer to a valiable to store the value */
	)
{
	unsigned long val;
	unsigned char r, s = 0;
	TCHAR c;


	*res = 0;
	while((c = **str) == ' ') (*str)++;	/* Skip leading spaces */

	if(c == '-') {		/* negative? */
		s = 1;
		c = *(++(*str));
	}

	if(c == '0') {
		c = *(++(*str));
		switch(c) {
		case 'x':		/* hexdecimal */
			r = 16; c = *(++(*str));
			break;
		case 'b':		/* binary */
			r = 2; c = *(++(*str));
			break;
		default:
			if(c <= ' ') return 1;	/* single zero */
			if(c < '0' || c > '9') return 0;	/* invalid char */
			r = 8;		/* octal */
		}
	}
	else {
		if(c < '0' || c > '9') return 0;	/* EOL or invalid char */
		r = 10;			/* decimal */
	}

	val = 0;
	while(c > ' ') {
		if(c >= 'a') c -= 0x20;
		c -= '0';
		if(c >= 17) {
			c -= 7;
			if(c <= 9) return 0;	/* invalid char */
		}
		if(c >= r) return 0;		/* invalid char for current radix */
		val = val * r + c;
		c = *(++(*str));
	}
	if(s) val = 0 - val;			/* apply sign if needed */

	*res = val;
	return 1;
}


/*----------------------------------------------*/
/* Dump a block of byte array                   */

void put_dump(
	const unsigned char* buff,	/* Pointer to the byte array to be dumped */
	unsigned long addr,			/* Heading address value */
	int cnt						/* Number of bytes to be dumped */
	)
{
	int i;


	_tprintf(_T("%08lX:"), addr);

	for(i = 0; i < cnt; i++)
		_tprintf(_T(" %02X"), buff[i]);

	_puttchar(' ');
	for(i = 0; i < cnt; i++)
		_puttchar((TCHAR)((buff[i] >= ' ' && buff[i] <= '~') ? buff[i] : '.'));

	_puttchar('\n');
}

void put_rc(const TCHAR *func, FRESULT rc)
{
	const TCHAR *p =
		_T("OK\0DISK_ERR\0INT_ERR\0NOT_READY\0NO_FILE\0NO_PATH\0INVALID_NAME\0")
		_T("DENIED\0EXIST\0INVALID_OBJECT\0WRITE_PROTECTED\0INVALID_DRIVE\0")
		_T("NOT_ENABLED\0NO_FILE_SYSTEM\0MKFS_ABORTED\0TIMEOUT\0LOCKED\0")
		_T("NOT_ENOUGH_CORE\0TOO_MANY_OPEN_FILES\0");
	FRESULT i;

	for(i = 0; i != rc && *p; i++) {
		while(*p++);
	}
	_tprintf(_T("%s() =>%u FR_%s\n"), func, (UINT)rc, p);
}

void xcopy(TCHAR *path)
{
	TCHAR subpath[_MAX_PATH];
	TCHAR temp[_MAX_PATH];
	WIN32_FIND_DATA lp;
	FRESULT res;
	FIL file[2];			/* File objects */
	DWORD len, wlen;
	UINT s2, cnt;
	HANDLE h;
	FILINFO fi;
	SYSTEMTIME st;
	z_stream stream;

	_tcscpy_s(temp, sizeof(temp), path);
	h = FindFirstFile(temp, &lp);
	if(h == INVALID_HANDLE_VALUE)
		return;

	temp[strlen(temp) - 1] = '\0';
	do
	{
		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			&& strcmp(lp.cFileName, "..") != 0 && strcmp(lp.cFileName, ".") != 0)
		{
			if((res = f_mkdir(lp.cFileName)) != RES_OK)
				put_rc("f_mkdir", res);

			FileTimeToSystemTime(&lp.ftLastWriteTime, &st);
			fi.fdate = (WORD)(((st.wYear - 1980) * 512U) | st.wMonth * 32U | st.wDay);
			fi.ftime = (WORD)(st.wHour * 2048U | st.wMinute * 32U | st.wSecond / 2U);
			if((res = f_utime(lp.cFileName, &fi)) != RES_OK)
				put_rc("f_utime", res);

			_stprintf_s(subpath, sizeof(subpath), _T("%s%s\\*"), temp, lp.cFileName);
			if((res = f_chdir(lp.cFileName)) != RES_OK)
				put_rc("f_chdir", res);
			xcopy(subpath);
			if((res = f_chdir(_T(".."))) != RES_OK)
				put_rc("f_chdir", res);
		}
		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			HANDLE fh;

			res = -1;
			_stprintf_s(subpath, sizeof(subpath), "%s%s", temp, lp.cFileName);
			fh = CreateFile(subpath, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
			if(fh != INVALID_HANDLE_VALUE) {
				if(ReadFile(fh, InBuff, sizeof InBuff, &len, 0))
					res = FR_OK;
				CloseHandle(fh);
			}

			while (res == FR_OK) {
				/* ˆ³k */
				memset(&stream, 0, sizeof(stream));
				/* allocate deflate memory, set up for gzip compression */
				stream.zalloc = Z_NULL;
				stream.zfree = Z_NULL;
				stream.opaque = Z_NULL;
				stream.next_in = InBuff;
				stream.avail_in = len;
				stream.next_out = OutBuff;
				stream.avail_out = sizeof(OutBuff);
				res = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
					MAX_WBITS + 16, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);
				if (res != Z_OK) {
					_tprintf(_T("out of memory %d"), res);
					break;
				}

				res = deflate(&stream, Z_FULL_FLUSH);
				if (res == Z_STREAM_ERROR) {
					_tprintf(_T("internal error: deflate stream corrupt %d"), res);
					break;
				}

				deflateEnd(&stream);

				len = stream.total_out;
				break;
			}

			if(res == FR_OK){
				if((res = f_open(&file[0], lp.cFileName, FA_CREATE_ALWAYS | FA_WRITE)) != RES_OK)
					put_rc("f_open", res);
				wlen = 0;
				while(len) {
					if((UINT)len >= sizeof OutBuff) {
						cnt = sizeof OutBuff;
						len -= sizeof OutBuff;
					}
					else {
						cnt = len;
						len = 0;
					}
					res = f_write(&file[0], OutBuff, cnt, &s2);
					if(res != FR_OK) {
						put_rc("f_write", res);
						break;
					}
					wlen += s2;
					if(cnt != s2)
						break;
				}
				_tprintf(_T("%lu bytes written.\n"), wlen);
				if((res = f_close(&file[0])) != RES_OK)
					put_rc("f_close", res);

				if((res = f_chmod(lp.cFileName, AM_RDO | AM_ARC, AM_RDO | AM_ARC | AM_SYS | AM_HID)) != RES_OK)
					put_rc("f_chmod", res);

				FileTimeToSystemTime(&lp.ftLastWriteTime, &st);
				fi.fdate = (WORD)(((st.wYear - 1980) * 512U) | st.wMonth * 32U | st.wDay);
				fi.ftime = (WORD)(st.wHour * 2048U | st.wMinute * 32U | st.wSecond / 2U);
				if((res = f_utime(lp.cFileName, &fi)) != RES_OK)
					put_rc("f_utime", res);
			}
		}
	} while(FindNextFile(h, &lp));

	FindClose(h);
}

FRESULT scan_files (
	char* path,        /* Start node to be scanned (also used as work area) */
	int size
)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	int i;
	char *fn;   /* This function assumes non-Unicode configuration */
#if _USE_LFN
	static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
	fno.lfname = lfn;
	fno.lfsize = sizeof lfn;
#endif

	res = f_opendir(&dir, path);                       /* Open the directory */
	if (res == FR_OK) {
		i = strlen(path);
		for (;;) {
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
			if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			fn = fno.fname;
#endif
			if (fno.fattrib & AM_DIR) {                    /* It is a directory */
				sprintf_s(&path[i], size -i, "/%s", fn);
				res = scan_files(path, size);
				path[i] = 0;
				if (res != FR_OK) break;
			} else {                                       /* It is a file. */
				printf("%s/%s\n", path, fn);
			}
		}
		f_closedir(&dir);
	}

	return res;
}

int _tmain(int argc, TCHAR *argv[])
{
	TCHAR pool[50];
	FRESULT res;
	BYTE pdrv = 0;
	WORD w;
	DWORD dw;
	char path[256] = ".";
	FATFS *fs;
	DWORD fre_clust, fre_sect, tot_sect;

	_stprintf_s(pool, sizeof(pool), _T(".%u"), _CODE_PAGE);
	_tsetlocale(LC_CTYPE, pool);

	_tprintf(_T("FatFs module test monitor (%s, CP:%u/%s)\n\n"),
		_USE_LFN ? _T("LFN") : _T("SFN"),
		_CODE_PAGE,
		_LFN_UNICODE ? _T("Unicode") : _T("ANSI"));

	_stprintf_s(pool, sizeof(pool), _T("FatFs debug console (%s, CP:%u/%s)"),
		_USE_LFN ? _T("LFN") : _T("SFN"),
		_CODE_PAGE,
		_LFN_UNICODE ? _T("Unicode") : _T("ANSI"));
	SetConsoleTitle(pool);

	assign_drives();	/* Find physical drives on the PC */

#if _MULTI_PARTITION
	_tprintf(_T("\nMultiple partition feature is enabled. Each logical drive is tied to the patition as follows:\n"));
	for(cnt = 0; cnt < sizeof VolToPart / sizeof(PARTITION); cnt++) {
		const TCHAR *pn[] = { _T("auto detect"), _T("1st partition"), _T("2nd partition"), _T("3rd partition"), _T("4th partition") };

		_tprintf(_T("\"%u:\" <== Disk# %u, %s\n"), cnt, VolToPart[cnt].pd, pn[VolToPart[cnt].pt]);
	}
	_tprintf(_T("\n"));
#else
	_tprintf(_T("\nMultiple partition feature is disabled.\nEach logical drive is tied to the same physical drive number.\n\n"));
#endif

#if _USE_LFN
	Finfo.lfname = LFName;
	Finfo.lfsize = sizeof LFName;
#endif

	res = disk_initialize(pdrv);
	_tprintf(_T("rc=%d\n"), res);
	if(disk_ioctl(pdrv, GET_SECTOR_SIZE, &w) == RES_OK)
		_tprintf(_T("Sector size = %u\n"), w);
	if(disk_ioctl(pdrv, GET_SECTOR_COUNT, &dw) == RES_OK)
		_tprintf(_T("Number of sectors = %u\n"), dw);

	if((res = f_mount(&FatFs[0], _T(""), 0)) != RES_OK)
		put_rc("f_mount", res);

	if((res = f_mkfs(_T(""), 1, 0)) != RES_OK)
		put_rc("f_mkfs", res);

	xcopy(_T("httpd-fs\\*"));

	if(disk_ioctl(0, 201, _T("httpd-fs.bin")) == RES_OK)
		_tprintf(_T("Ok\n"));

	if((res = scan_files(path, sizeof(path))) != RES_OK)
		put_rc("scan_files", res);

	/* Get volume information and free clusters of drive 1 */
	if(f_getfree(_T("0:"), &fre_clust, &fs) == RES_OK){
		/* Get total sectors and free sectors */
		tot_sect = (fs->n_fatent - 2) * fs->csize;
		fre_sect = fre_clust * fs->csize;

		/* Print the free space (assuming 512 bytes/sector) */
		_tprintf(_T("%10lu KB total drive space.\n%10lu KB available.\n"),
			tot_sect / 2, fre_sect / 2);;
	}
}
