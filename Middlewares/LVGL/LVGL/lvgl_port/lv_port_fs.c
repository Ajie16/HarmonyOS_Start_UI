/**
 * @file lv_port_fs_templ.c
 *
 */

 /*Copy this file as "lv_port_fs.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_fs.h"
#include "fatfs.h"
#include "usart.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef FIL file_t;		// 把FIL类型定义成file_t
typedef DIR dir_t; 		// 把DIR类型定义成dir_t

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void fs_init(void);

static lv_fs_res_t fs_open (lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos);
static lv_fs_res_t fs_size (lv_fs_drv_t * drv, void * file_p, uint32_t * size_p);
static lv_fs_res_t fs_tell (lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);
static lv_fs_res_t fs_remove (lv_fs_drv_t * drv, const char *path);
static lv_fs_res_t fs_trunc (lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_rename (lv_fs_drv_t * drv, const char * oldname, const char * newname);
static lv_fs_res_t fs_free (lv_fs_drv_t * drv, uint32_t * total_p, uint32_t * free_p);
static lv_fs_res_t fs_dir_open (lv_fs_drv_t * drv, void * rddir_p, const char *path);
static lv_fs_res_t fs_dir_read (lv_fs_drv_t * drv, void * rddir_p, char *fn);
static lv_fs_res_t fs_dir_close (lv_fs_drv_t * drv, void * rddir_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_fs_init(void)
{
    /*----------------------------------------------------
     * Initialize your storage device and File System
     * -------------------------------------------------*/
    fs_init();

    /*---------------------------------------------------
     * Register the file system interface in LVGL
     *--------------------------------------------------*/

    /* Add a simple drive to open images */
    lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    /*Set up fields...*/
    fs_drv.file_size = sizeof(file_t);
    fs_drv.letter = 'S';
    fs_drv.open_cb = fs_open;
    fs_drv.close_cb = fs_close;
    fs_drv.read_cb = fs_read;
    fs_drv.write_cb = fs_write;
    fs_drv.seek_cb = fs_seek;
    fs_drv.tell_cb = fs_tell;
    fs_drv.free_space_cb = fs_free;
    fs_drv.size_cb = fs_size;
    fs_drv.remove_cb = fs_remove;
    fs_drv.rename_cb = fs_rename;
    fs_drv.trunc_cb = fs_trunc;

    fs_drv.rddir_size = sizeof(dir_t);
    fs_drv.dir_close_cb = fs_dir_close;
    fs_drv.dir_open_cb = fs_dir_open;
    fs_drv.dir_read_cb = fs_dir_read;

    lv_fs_drv_register(&fs_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* Initialize your Storage device and File system. */
static void fs_init(void)
{

    /*E.g. for FatFS initialize the SD card and FatFS itself*/
		static FATFS  fs;			/* FATFS 文件系统对象 */
		FRESULT fr; 		/* FATFS API 返回值 */
		printf("LVGL FATFS INIT...\r\n");
		fr = f_mount(&fs, "", 0);
		if (fr != FR_OK) 
		{
				printf("SD Card mounted error: (%d)\r\n", fr);
		} else
			printf("SD Card mounted successfully!\r\n");
    /*You code here*/
}

/**
 * Open a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param path path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_open (lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;
	  FRESULT fr; 		/* FATFS API 返回值 */
	
    if(mode == LV_FS_MODE_WR)
    {
        /*Open a file for write*/
				fr = f_open((FIL*)file_p, path, LV_FS_MODE_WR);
				if(fr != FR_OK)
				{
					printf("open file \"%s\" error : %d\r\n", path, fr);
				}else
				res = LV_FS_RES_OK;
    }
    else if(mode == LV_FS_MODE_RD)
    {
        /*Open a file for read*/
				fr = f_open((FIL*)file_p, path, LV_FS_MODE_RD);
				if(fr != FR_OK)
				{
					printf("open file \"%s\" error : %d\r\n", path, fr);
				}else
					res = LV_FS_RES_OK;
		}
    else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))
    {
        /*Open a file for read and write*/
				fr = f_open((FIL*)file_p, path, LV_FS_MODE_WR | LV_FS_MODE_RD);
				if(fr != FR_OK)
				{
					printf("open file \"%s\" error : %d\r\n", path, fr);
				}else
				res = LV_FS_RES_OK;
    }
    return res;
}

/**
 * Close an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;
		FRESULT fr; 		/* FATFS API 返回值 */
		printf("close file\r\n");
		/* 操作完成，关闭文件 */
		fr = f_close((FIL*)file_p);
		if(fr != FR_OK)
		{
			printf("close file error! error : %d\r\n", fr);
		}else
			res = LV_FS_RES_OK;
			return res;
}

/**
 * Read data from an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
  lv_fs_res_t res = LV_FS_RES_NOT_IMP;
	FRESULT fr; 		/* FATFS API 返回值 */

	fr = f_read((FIL*)file_p, buf, btr, br);
  if (fr != FR_OK) 
	{
    printf("read file error! error : %d\r\n", fr);
  }else
		res = LV_FS_RES_OK;
   return res;
}

/**
 * Write into a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param buf pointer to a buffer with the bytes to write
 * @param btr Bytes To Write
 * @param br the number of real written bytes (Bytes Written). NULL if unused.
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
  lv_fs_res_t res = LV_FS_RES_NOT_IMP;
	FRESULT fr; 		/* FATFS API 返回值 */
	fr = f_write((FIL*)file_p, buf, btw, bw);
  if (fr != FR_OK) 
	{
    	printf("read file error! error : %d\r\n", fr);
  }else
	res = LV_FS_RES_OK;

  return res;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos)
{
  lv_fs_res_t res = LV_FS_RES_NOT_IMP;
	FRESULT fr; 		/* FATFS API 返回值 */
	fr = f_lseek((FIL*)file_p, pos);
  if (fr != FR_OK) 
	{
    printf("read file error! error : %d\r\n", fr);
  }else
	res = LV_FS_RES_OK;
	
  return res;
}

/**
 * Give the size of a file bytes
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param size pointer to a variable to store the size
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_size (lv_fs_drv_t * drv, void * file_p, uint32_t * size_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}
/**
 * Give the position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell (lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Delete a file
 * @param drv pointer to a driver where this function belongs
 * @param path path of the file to delete
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_remove (lv_fs_drv_t * drv, const char *path)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Truncate the file size to the current position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to an 'ufs_file_t' variable. (opened with lv_fs_open )
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_trunc (lv_fs_drv_t * drv, void * file_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Rename a file
 * @param drv pointer to a driver where this function belongs
 * @param oldname path to the file
 * @param newname path with the new name
 * @return LV_FS_RES_OK or any error from 'fs_res_t'
 */
static lv_fs_res_t fs_rename (lv_fs_drv_t * drv, const char * oldname, const char * newname)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Get the free and total size of a driver in kB
 * @param drv pointer to a driver where this function belongs
 * @param letter the driver letter
 * @param total_p pointer to store the total size [kB]
 * @param free_p pointer to store the free size [kB]
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_free (lv_fs_drv_t * drv, uint32_t * total_p, uint32_t * free_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Initialize a 'lv_fs_dir_t' variable for directory reading
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to a 'lv_fs_dir_t' variable
 * @param path path to a directory
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_open (lv_fs_drv_t * drv, void * rddir_p, const char *path)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Read the next filename form a directory.
 * The name of the directories will begin with '/'
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to an initialized 'lv_fs_dir_t' variable
 * @param fn pointer to a buffer to store the filename
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_read (lv_fs_drv_t * drv, void * rddir_p, char *fn)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Close the directory reading
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to an initialized 'lv_fs_dir_t' variable
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_close (lv_fs_drv_t * drv, void * rddir_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
