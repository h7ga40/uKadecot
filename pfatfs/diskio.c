/*-----------------------------------------------------------------------*/
/* PFF - Low level disk control module for Win32        (C)ChaN, 2014    */
/*-----------------------------------------------------------------------*/

#include <string.h>
#include "pffconf.h"
#include "diskio.h"

/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

#define MAX_DRIVES	1	/* Max number of physical drives to be used */

#define SZ_RAMDISK	768	/* Size of RAM disk [kB] */
#define SS_RAMDISK	512	/* Sector size of RAM disk [byte] */

#ifndef _MSC_VER
#define RamDisk		((unsigned char *)0xfff00000)
#else
#include <windows.h>
#include <tchar.h>
#include <winioctl.h>

unsigned char RamDisk[SZ_RAMDISK * 1024];
#endif

typedef struct {
	DSTATUS	status;
	WORD sz_sector;
	DWORD wofs;
	DWORD wip;
	DWORD n_sectors;
} STAT;

static volatile
STAT Stat[MAX_DRIVES];

static
BYTE Buffer[SS_RAMDISK];

static
int get_status(volatile STAT *stat) {
	stat->sz_sector = SS_RAMDISK;
	if(stat->sz_sector < _MIN_SS || stat->sz_sector > _MAX_SS) return 0;
	stat->n_sectors = SZ_RAMDISK * 1024 / SS_RAMDISK;
	stat->status = 0;
	return 1;
}

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (void)
{
	DSTATUS sta;
#ifdef _MSC_VER
	HANDLE h;
	DWORD br;

	h = CreateFile(_T("..\\..\\..\\..\\uip\\apps\\webserver\\httpd-fs.bin"), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (h != INVALID_HANDLE_VALUE) {
		ReadFile(h, RamDisk, sizeof(RamDisk), &br, 0);
		CloseHandle(h);
	}
#endif
	get_status(&Stat[0]);
	sta = Stat[0].status;
	Stat[0].wip = 0;

	return sta;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS disk_get_status (void)
{
	return Stat[0].status;
}

/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/
DRESULT disk_readp (
	BYTE* buff,		/* Data read buffer */
	DWORD sector,	/* Sector number (LBA) */
	UINT offset,	/* Offset in the sector */
	UINT count		/* Byte count */
)
{
	DRESULT res;
	int dofs;


	if(Stat[0].status & STA_NOINIT) return RES_NOTRDY;

	if(sector >= Stat[0].n_sectors || !count || offset + count > Stat[0].sz_sector/* || Stat[0].wip*/) {
		res = RES_PARERR;
	}
	else {
		if(buff) {
			dofs = sector * Stat[0].sz_sector;
			memcpy(buff, &RamDisk[dofs + offset], count);
			res = RES_OK;
		}
		else {
			res = RES_OK;
		}
	}

	return res;
}

/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/
DRESULT disk_writep (
	const BYTE* buff,	/* Pointer to the write data */
	DWORD sc			/* Sector number (LBA), Number of bytes to send */
)
{
	DRESULT res;
	int dofs;

	if(Stat[0].status & STA_NOINIT) return RES_NOTRDY;

	if(!buff) {
		if(sc) {
			Stat[0].wip = sc;
			Stat[0].wofs = 0;
			memset(Buffer, 0, sizeof Buffer);
			res = RES_OK;
		}
		else {
			if(Stat[0].wip) {
				dofs = Stat[0].wip * Stat[0].sz_sector;
				memcpy(RamDisk + dofs, Buffer, Stat[0].sz_sector);
				Stat[0].wip = 0;
				res = RES_OK;
			}
			else {
				res = RES_OK;
			}
		}
	}
	else {
		if(Stat[0].wofs + sc > Stat[0].sz_sector) sc = Stat[0].sz_sector - Stat[0].wofs;
		memcpy(&Buffer[Stat[0].wofs], buff, sc);
		Stat[0].wofs += sc;
		res = RES_OK;
	}

	return res;
}
