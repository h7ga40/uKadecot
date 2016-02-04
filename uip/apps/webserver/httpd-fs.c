/*
 * Copyright (c) 2001, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id$
 */

#include "httpd.h"
#include "httpd-fs.h"
#include "pff.h"
#include "diskio.h"
#include "mmc_rspi.h"
#include <string.h>
#include "http-strings.h"

#ifndef _MSC_VER
#ifndef strcat_s
#define strcat_s(dst, dsz, src) strcat(dst, src)
#endif
#endif

//#define FILE_DUMP
#ifdef FILE_DUMP
char path[256] = "";
FATFS fs;
#endif

#ifndef NULL
#define NULL 0
#endif /* NULL */

/*-----------------------------------------------------------------------------------*/
#ifdef FILE_DUMP
FRESULT scan_files (FATFS *fs, char* path, int size)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	int i;
	char *fn;   /* This function assumes non-Unicode configuration */
#if _USE_LFN
	char LFName[256];
	fno.lfname = LFName;
	fno.lfsize = sizeof(LFName);
#endif

	dir.fs = fs;
	res = pf_opendir(&dir, path);
	if (res == FR_OK) {
		i = strlen(path);
		for (;;) {
			res = pf_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) break;
#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			fn = fno.fname;
#endif
			if (fno.fattrib & AM_DIR) {
				sprintf_s(&path[i], size - i, "/%s", fn);
				res = scan_files(fs, path, size);
				if (res != FR_OK) break;
				path[i] = 0;
			} else {
				printf("%s/%s\n", path, fn);
			}
		}
	}

	return res;
}
#endif
/*-----------------------------------------------------------------------------------*/
void
httpd_fs_init(void)
{
#ifdef FILE_DUMP
	FRESULT res;
#endif
	ramdisk_initialize();
	mmc_rspi_initialize();
#ifdef FILE_DUMP
	if((res = pf_mount(&fs)) != FR_OK){
		printf("pf_mount() => %d\n", res);
		return;
	}
	if((res = scan_files(&fs, path, sizeof(path))) != FR_OK){
		printf("scan_files() => %d\n", res);
		return;
	}
#endif
}
/*-----------------------------------------------------------------------------------*/
int
httpd_fs_open(int drv, char *name, int len, struct httpd_fs_file *file)
{
	FRESULT res;
	FATFS *fs = (FATFS *)&file->fs;
	FILINFO fno;
	DIR dir;

	file->pos = 0;
	file->len = 0;
	file->drv = drv;
	file->name = name;
	file->redirect = 0;
	memset(fs, 0, sizeof(FATFS));

	if (drv == 0) {
		fs->disk_get_status = ramdisk_get_status;
		fs->disk_readp = ramdisk_readp;
		fs->disk_writep = ramdisk_writep;
	}
	else {
		fs->disk_get_status = mmc_rspi_get_status;
		fs->disk_readp = mmc_rspi_readp;
		fs->disk_writep = mmc_rspi_writep;
	}

	if (fs->fs_type == 0){
		if((res = pf_mount(fs)) != FR_OK){
			printf("pf_mount() => %d\n", res);
			return 0;
		}
	}

	if ((res = pf_open(fs, name)) != FR_OK) {
		dir.fs = fs;
		if ((res = pf_opendir(&dir, name)) != FR_OK) {
			printf("pf_opendir(%d:%s) => %d\n", drv, name, res);
			return 0;
		}

		if ((res = pf_readdir(&dir, &fno)) != FR_OK) {
			printf("pf_readdir(%d:%s) => %d\n", drv, name, res);
			return 0;
		}

		if (len != 0/*fno.fattrib & AM_DIR*/) {
			strcat_s(name, len, http_index_html);
			res = pf_open(fs, name);
			file->redirect = res == FR_OK;
		}
		else
			res = FR_NO_FILE;

		if (res != FR_OK) {
			printf("pf_open(%d:%s) => %d %x\n", drv, name, res, fno.fattrib);
			return 0;
		}
	}

	file->len = fs->fsize;

	//printf("httpd_fs_open(%d:%s) %d\n", drv, name, file->len);

	return 1;
}
/*-----------------------------------------------------------------------------------*/
int
httpd_fs_read(struct httpd_fs_file *file, void *dst, int len)
{
	FRESULT ret;
	UINT rlen;
	FATFS *fs = (FATFS *)&file->fs;

	if((ret = pf_lseek(fs, file->pos)) != FR_OK){
		printf("pf_lseek(%d:%s, %d) => %d\n", file->drv, file->name, file->pos, ret);
		return 0;
	}

	if(file->pos != fs->fptr){
		printf("pf_lseek(%d:%s, %d) != %d\n", file->drv, file->name, file->pos, fs->fptr);
	}

	if((ret = pf_read(fs, dst, len, &rlen)) != FR_OK){
		printf("pf_read(%d:%s, 0x%p, %d) => %d\n", file->drv, file->name, dst, len, ret);
		return 0;
	}

	//printf("httpd_fs_read(%d:%s, %d, %d) => %d\n", file->drv, file->name, file->pos, len, rlen);

	return rlen;
}
/*-----------------------------------------------------------------------------------*/
