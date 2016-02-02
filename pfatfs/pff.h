/*---------------------------------------------------------------------------/
/  Petit FatFs - FAT file system module include file  R0.03   (C)ChaN, 2014
/----------------------------------------------------------------------------/
/ Petit FatFs module is an open source software to implement FAT file system to
/ small embedded systems. This is a free software and is opened for education,
/ research and commercial developments under license policy of following trems.
/
/  Copyright (C) 2014, ChaN, all right reserved.
/
/ * The Petit FatFs module is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial use UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/----------------------------------------------------------------------------*/

#ifndef _PFATFS
#define _PFATFS	4004	/* Revision ID */

#ifdef __cplusplus
extern "C" {
#endif

#include "integer.h"
#include "pffconf.h"
#include "diskio.h"

#if _PFATFS != _PFFCONF
#error Wrong configuration file (pffconf.h).
#endif

#if _FS_FAT32
#define	CLUST	DWORD
#else
#define	CLUST	WORD
#endif


/* File system object structure */

typedef struct {
	DSTATUS (* disk_get_status)(void);
	DRESULT (* disk_readp)(BYTE* buff, DWORD sector, UINT offset, UINT count);
	DRESULT (* disk_writep)(const BYTE* buff, DWORD sc);
	BYTE	fs_type;	/* FAT sub type */
	BYTE	flag;		/* File status flags */
	BYTE	csize;		/* Number of sectors per cluster */
	BYTE	pad1;
	WORD	n_rootdir;	/* Number of root directory entries (0 on FAT32) */
	CLUST	n_fatent;	/* Number of FAT entries (= number of clusters + 2) */
	DWORD	fatbase;	/* FAT start sector */
	DWORD	dirbase;	/* Root directory start sector (Cluster# on FAT32) */
	DWORD	database;	/* Data start sector */
	DWORD	fptr;		/* File R/W pointer */
	DWORD	fsize;		/* File size */
	CLUST	org_clust;	/* File start cluster */
	CLUST	curr_clust;	/* File current cluster */
	DWORD	dsect;		/* File current data sector */
#if _USE_LFN
	WCHAR LfnBuf[_MAX_LFN + 1];
#endif
} FATFS;



/* Directory object structure */

typedef struct {
	FATFS*	fs;			/* Pointer to the owner file system object (**do not change order**) */
	WORD	index;		/* Current read/write index number */
	BYTE*	fn;			/* Pointer to the SFN (in/out) {file[8],ext[3],status[1]} */
	CLUST	sclust;		/* Table start cluster (0:Static table) */
	CLUST	clust;		/* Current cluster */
	DWORD	sect;		/* Current sector */
#if _USE_LFN
	WCHAR*	lfn;		/* Pointer to the LFN working buffer */
	WORD	lfn_idx;	/* Last matched LFN index number (0xFFFF:No LFN) */
#endif
} DIR;



/* File status structure */

typedef struct {
	DWORD	fsize;		/* File size */
	WORD	fdate;		/* Last modified date */
	WORD	ftime;		/* Last modified time */
	BYTE	fattrib;	/* Attribute */
	char	fname[13];	/* File name */
#if _USE_LFN
	char*	lfname;		/* Pointer to the LFN buffer */
	UINT 	lfsize;		/* Size of LFN buffer in TCHAR */
#endif
} FILINFO;



/* File function return code (FRESULT) */

typedef enum {
	FR_OK = 0,			/* (0) Succeeded */
	FR_DISK_ERR,		/* (1) A hard error occurred in the low level disk I/O layer */
	FR_NOT_READY,		/* (2) The physical drive cannot work */
	FR_NO_FILE,			/* (3) Could not find the file */
	FR_NO_PATH,			/* (4) Could not find the path */
	FR_INVALID_NAME,	/* (5) The path name format is invalid */
	FR_NOT_OPENED,		/* (6) */
	FR_NOT_ENABLED,		/* (7) */
	FR_NO_FILESYSTEM,	/* (8) There is no valid FAT volume */
} FRESULT;



/*--------------------------------------------------------------*/
/* Petit FatFs module application interface                     */

FRESULT pf_mount (FATFS* fs);										/* Mount/Unmount a logical drive */
FRESULT pf_open (FATFS* fs, const char* path);						/* Open a file */
FRESULT pf_read (FATFS* fs, void* buff, UINT btr, UINT* br);		/* Read data from the open file */
FRESULT pf_write (FATFS* fs, const void* buff, UINT btw, UINT* bw);	/* Write data to the open file */
FRESULT pf_lseek (FATFS* fs, DWORD ofs);							/* Move file pointer of the open file */
FRESULT pf_opendir (DIR* dj, const char* path);			/* Open a directory */
FRESULT pf_readdir (DIR* dj, FILINFO* fno);				/* Read a directory item from the open directory */



/*--------------------------------------------------------------*/
/* Flags and offset address                                     */

/* File status flag (FATFS.flag) */

#define	FA_OPENED	0x01
#define	FA_WPRT		0x02
#define	FA__WIP		0x40


/* FAT sub type (FATFS.fs_type) */

#define FS_FAT12	1
#define FS_FAT16	2
#define FS_FAT32	3


/* File attribute bits for directory entry */

#define	AM_RDO	0x01	/* Read only */
#define	AM_HID	0x02	/* Hidden */
#define	AM_SYS	0x04	/* System */
#define	AM_VOL	0x08	/* Volume label */
#define AM_LFN	0x0F	/* LFN entry */
#define AM_DIR	0x10	/* Directory */
#define AM_ARC	0x20	/* Archive */
#define AM_MASK	0x3F	/* Mask of defined bits */


/*--------------------------------*/
/* Multi-byte word access macros  */

#if _WORD_ACCESS == 1	/* Enable word access to the FAT structure */
#define	LD_WORD(ptr)		(WORD)(*(WORD*)(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(*(DWORD*)(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(WORD*)(BYTE*)(ptr)=(WORD)(val)
#define	ST_DWORD(ptr,val)	*(DWORD*)(BYTE*)(ptr)=(DWORD)(val)
#else					/* Use byte-by-byte access to the FAT structure */
#define	LD_WORD(ptr)		(WORD)(((WORD)*((BYTE*)(ptr)+1)<<8)|(WORD)*(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(((DWORD)*((BYTE*)(ptr)+3)<<24)|((DWORD)*((BYTE*)(ptr)+2)<<16)|((WORD)*((BYTE*)(ptr)+1)<<8)|*(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8)
#define	ST_DWORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8); *((BYTE*)(ptr)+2)=(BYTE)((DWORD)(val)>>16); *((BYTE*)(ptr)+3)=(BYTE)((DWORD)(val)>>24)
#endif


#ifdef __cplusplus
}
#endif

#endif /* _PFATFS */
