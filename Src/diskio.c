/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* Declarations of disk functions */
#include "SD.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

extern SPI_HandleTypeDef hspi3;
SD_Card sdCard = { .spi = &hspi3, .cs = { .PORT = SDSPI_CS_GPIO_Port, .PIN = SDSPI_CS_Pin } };

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	if (pdrv)
		return STA_NOINIT;		/* Supports only single drive */
	return SD_Status;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	if (pdrv)
		return STA_NOINIT;			/* Supports only single drive */
	SD_CardInit(&sdCard);
	return SD_Status;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	if(pdrv)
		return RES_ERROR;
	return SD_disk_read(&sdCard, buff, sector, count);
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	if(pdrv)
		return RES_ERROR;
	return SD_disk_write(&sdCard, buff, sector, count);
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	if(SD_Status != 0) return RES_NOTRDY;    //Pamiêæ niezainicjowana

	DRESULT res=RES_OK;

	switch(cmd)
	{
		case GET_SECTOR_SIZE:
			*((DWORD*)buff)=512;         //D³ugoœæ sektora 512 bajtów
			break;
		case GET_SECTOR_COUNT:
		{
			char buf[16];
			SD_CSDRegv1 *csd=(SD_CSDRegv1*)buf;
			uint8_t r1=SD_SendCMD(&sdCard, CMD9, 0);  //Odczytaj CSD
			if(r1==0)
			{
				SD_GetResponse(&sdCard, csd->Reg, 16);
				uint64_t size;  //Rozmiar karty
				if(csd->CSD_Struct == 0)
					size=(csd->C_Size + 1) * (1ULL << csd->Read_Bl_Len) * (1UL << (csd->C_Size_mult + 2));
				else
					size=(((SD_CSDRegv2*)csd)->C_Size + 1) * 524288ULL;  //C_Size zawiera liczbê bloków po 512 kB
				*((DWORD*)buff)=size/512;      //Zwracami liczbê 512-bajtowych sektorów
			}
			else
				res=RES_NOTRDY;
		}
			break;
		case GET_BLOCK_SIZE:
			*((DWORD*)buff)=1;           //Ile mo¿na na raz skasowaæ sektorów
			break;
		case CTRL_SYNC:	break;
		case CTRL_TRIM:	break;
		default:
			res=RES_PARERR;              //Nieznane polecenie
	}

	return res;
}


DWORD get_fattime (void)
{
	return 0;  //B³êdnie, nale¿y tu zwróciæ poprawn¹ datê
}

