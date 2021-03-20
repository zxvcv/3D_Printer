/*
 * SD.c
 *
 * Created: 2013-07-10 11:47:53
 *  Author: tmf
 */

#include "SD.h"
#include "diskio.h"
#include <stdbool.h>
#include <string.h>

uint8_t SD_Status;               //Co aktualnie jest podpi�te
uint8_t SD_Type;                 //Typ karty - SD lub SDHC

void SPI_SD_CS(SD_Card* sd, _Bool SS_en){
	if(SS_en){
		HAL_GPIO_WritePin(sd->cs.PORT, sd->cs.PIN, GPIO_PIN_RESET);	//Aktywuj kart� SD
		SD_WaitForReady(sd);       //Zaczekaj a� karta b�dzie gotowa
	} else {
		HAL_GPIO_WritePin(sd->cs.PORT, sd->cs.PIN, GPIO_PIN_SET);
		SPI_RW_Byte(sd, 0xff);        //Karta ulega deaktywacji po otrzymaniu kolejnego zbocza sygna�u SCK
	}
}

void SPI_SetInitBaudrate(SD_Card* sd)
{
	if (HAL_SPI_DeInit(sd->spi) != HAL_OK) {
		Error_Handler();
	}
	sd->spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	if (HAL_SPI_Init(sd->spi) != HAL_OK) {
		Error_Handler();
	}
}

void SPI_SetMaxBaudrate(SD_Card* sd)
{
	if (HAL_SPI_DeInit(sd->spi) != HAL_OK) {
		Error_Handler();
	}
	sd->spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	if (HAL_SPI_Init(sd->spi) != HAL_OK) {
		Error_Handler();
	}
}

uint8_t SPI_RW_Byte(SD_Card* sd, uint8_t byte)
{
	uint8_t rx;
	HAL_SPI_TransmitReceive(sd->spi, &byte, &rx, 1, 500);
	return rx;
}

_Bool SD_WaitForReady(SD_Card* sd)
{
	uint16_t counter=500;              //Czekamy maksymalnie ok. 500 ms
	SPI_RW_Byte(sd, 0xff);
	do									//Zaczekaj na gotowo�� karty - sygna� CS musi by� wcze�niej aktywowany
	{
		if(SPI_RW_Byte(sd, 0xff) == 0xff) break;
		HAL_Delay(1);
	} while (--counter);
	return counter ? true : false;      //Czy wyst�pi� timeout?
}

void SD_CRC7(uint8_t *crc, uint8_t byte)  //Wylicza CRC7 dla karty, pocz�tkowo crc=0
{
	for (uint8_t i=0; i < 8; i++)
	{
		*crc <<= 1;
		if ((((byte & 0x80) ^ (*crc & 0x80)) != 0))
		{
			*crc ^= 0x09;
		}
		byte <<= 1;
	}
}

uint8_t SD_SendCMD(SD_Card* sd, uint8_t cmd, uint32_t arg)
{
	uint8_t crc=0, res;

	if(cmd & 0x80)
	{
		cmd&=0x7F;            //Skasuj najstarszy bit polecenia
		res=SD_SendCMD(sd, CMD55, 0);  //Kolejne polecenie nale�y do grupy ACMD
		if(res > 0x01) return res; //Polecenie CMD55 zako�czy�o si� b��dem
	}
	SPI_SD_CS(sd, false);         //Deaktywuj kart�
	SPI_SD_CS(sd, true);          //Aktywuj kart�

	cmd|=0x40;                //najstarsze dwa bity zawsze s� r�wne 01
	SPI_RW_Byte(sd, cmd);
	SD_CRC7(&crc, cmd);
	for(uint8_t i=0; i<4; i++)
	{
		SPI_RW_Byte(sd, (arg >> 24) & 0xff);
		SD_CRC7(&crc, (arg >> 24) & 0xff);
		arg<<=8;
	}
	crc=(crc << 1) | 1;        //CRC7 dla SD jest przesuni�te o jeden bit w lewo i ma ustawiony najm�odszy bit
	SPI_RW_Byte(sd, crc);          //Wy�lij CRC polecenia

	uint8_t i=10;         //Odpowied� mo�e nadej�� od 1 do 10 bajt�w po poleceniu
	do
	{
		res=SPI_RW_Byte(sd, 0xff);
	} while ((res & 0x80) && --i);
	return res;                //Po wys�aniu polecenia karta pozostaje wybrana w celu odczytu/zapisu kolejnych bajt�w
}

uint32_t SD_GetR7(SD_Card* sd)  //Pobierz odpowied� typu R7
{
	uint32_t R7=0;
	for(uint8_t i=0; i<4; i++)
	{
		R7<<=8;
		R7|=SPI_RW_Byte(sd, 0xff);
	}
	return R7;
}

_Bool SD_GetResponse(SD_Card* sd, void *buf, uint8_t size)
{
	uint8_t i=10;
	while((SPI_RW_Byte(sd, 0xff)!=0xfe) && (--i));
	if(!i) return false;
	while(size)
	{
		((uint8_t*)buf)[size-1]=SPI_RW_Byte(sd, 0xff);
		--size;
	}
	return true;
}

_Bool SD_CardInitV1(SD_Card* sd)
{
	uint8_t i;
	for(i=0; i<255; i++)
	{
	  if(SD_SendCMD(sd, ACMD41, 0) == 0) break;  //Karta w tryb aktywny
	  else HAL_Delay(10);
	}
	if(i == 255) return false;      //Karta nie akceptuje polecenia - uszkodzona?
	SD_GetR7(sd);                     //Co prawda mamy odpowied� R3, ale jej d�ugo�� jest taka sama jak R7
	SD_SendCMD(sd, CMD16, 512);         //D�ugo�� bloku 512 bajt�w
	SD_Status=0;
	SD_Type=CT_SD1;
	return true;
}

_Bool SD_CardInitV2(SD_Card* sd)
{
	uint8_t i;

	if(SD_GetR7(sd) != 0x1aa)
	 return false;  //B�edne napi�cie pracy karty

	for(i=0; i<255; i++)
	{
		if(SD_SendCMD(sd, ACMD41, SD_OCR_CCS) == 0) break;  //Karta w tryb aktywny
		else HAL_Delay(40);

	}
	if(i == 255) return false;           //Karta nie akceptuje polecenia - uszkodzona?
	if(SD_SendCMD(sd, CMD58, 0)==0)
	{
		SD_Status=0;
		if(SD_GetR7(sd) & SD_OCR_CCS) SD_Type=CT_SD2;  //Pobierz OCR
		    else SD_Type=CT_SD1;
	}
	return true;
}

_Bool SD_CardInit(SD_Card* sd)
{
	SPI_SetInitBaudrate(sd);     //Kart� nale�y zainicjowa� przy SCK r�wnym 400 kHz
	SPI_SD_CS(sd, false);                      //Deaktywuj kart�

	for(uint8_t i=0; i<10; i++)
	{
		SPI_RW_Byte(sd, 0xff);                 //Wy�lij co najmniej 74 takty SCK dla inicjalizacji karty
	}

	uint8_t i=10, ret;
	do
	{
		ret=SD_SendCMD(sd, CMD0, 0);

	} while ((ret != SD_R1_Idle) && --i);  //Zaczekaj na prawid�ow� odpowied� ze strony karty

	if(!i)
	{
		SD_Status=STA_NODISK;              //Brak karty lub inny b��d
		return false;                      //Niedoczekali�my si�
	}

	i=10;
	do
	{
		ret=SD_SendCMD(sd, CMD8, 0x1aa);
	} while ((ret & 0xc0) && --i);

	_Bool result;
	if(ret==0x05) result=SD_CardInitV1(sd); else result=SD_CardInitV2(sd); //Z jak� kart� mamy do czynienia
	if(result==false) SD_Status=STA_NODISK;
	SPI_SetMaxBaudrate(sd);  //Prze��cz interfejs na maksymaln� szybko�� pracy
	return result;
}

_Bool SD_Rec_Datablock(SD_Card* sd, uint8_t *buff, uint16_t size)
{
	uint8_t rec, timeout;

	timeout=200;
	do
	{
		HAL_Delay(1);
		//_delay_us(500);                  //Zanim blok danych b�dzie gotowy potrzebujemy chwilk�
		rec=SPI_RW_Byte(sd, 0xff);
	} while((rec == 0xff) && timeout--); //Czekamy na otrzymanie 0xfe, ale max 100 ms

	if(rec != 0xfe) return false;		 //B��d - nie ma pocz�tku danych

	do {						         //Odbierz blok danych
		*buff=SPI_RW_Byte(sd, 0xff);
		buff++;
	} while(--size);
	SPI_RW_Byte(sd, 0xff);                   //Odbierz CRC bloku danych - obowi�zkowe
	SPI_RW_Byte(sd, 0xff);
	return true;
}

uint8_t SD_disk_read(SD_Card* sd, uint8_t *buff, uint32_t sector, uint8_t count)
{
	if(SD_Status != 0) return RES_NOTRDY;
	if(SD_Type == CT_SD1) sector*=512;	//SDSC adresowana jest bajtami

	if(count == 1)
	{	//Odczytujemy pojedynczy sektor
		if((SD_SendCMD(sd, CMD17, sector) == 0) && SD_Rec_Datablock(sd, buff, 512))
		count=0;
	} else
	{				//Odczytujemy kilka sektor�w na raz
		if(SD_SendCMD(sd, CMD18, sector) == 0)
		{
			do
			{
				if(!SD_Rec_Datablock(sd, buff, 512)) break;
				buff+=512;
			} while(--count);
			SD_SendCMD(sd, CMD12, 0);				//Koniec transmisji
		}
	}

	SPI_SD_CS(sd, false);

	return count ? RES_ERROR : RES_OK;
}

_Bool SD_Sendblock(SD_Card* sd, const uint8_t *buff, uint8_t token)  //Wy�lij 512-bajtowy blok danych i zako�cz go tokenem
{
	uint8_t resp;
	uint16_t counter;

	if(SD_WaitForReady(sd) == false) return false; //Zaczekaj na gotowo�� karty

	SPI_RW_Byte(sd, token);					//Wy�lij token pocz�tku/ko�ca danych
	if(token != 0xfd)                   //0xfd to token ko�ca danych - po nim nic nie wysy�amy
	{
		counter=512;
		do
		{
			SPI_RW_Byte(sd, *buff++);       //Wy�lij dane
		} while(--counter);
		SPI_RW_Byte(sd, 0xff);				//Musimy wys�a� CRC16, nawet je�li karta go nie sprawdza
		SPI_RW_Byte(sd, 0xff);
		resp=SPI_RW_Byte(sd, 0xff);			//Odczytaj odpoweid� karty
		if((resp & 0x1F) != 0x05) return false;	//Sprawd� czy karta zaakceptowa�a dane
	}
	return true;
}

uint8_t SD_disk_write(SD_Card* sd, const uint8_t *buff, uint32_t sector, uint8_t count)
{
	if(SD_Status != 0) return RES_NOTRDY;
	if(SD_Type == CT_SD1) sector*=512;	//SDSC adresowana jest bajtami

	if(count == 1)
	{	//Zapisujemy pojedynczy sektor
		if((SD_SendCMD(sd, CMD24, sector) == 0) && SD_Sendblock(sd, buff, 0xfe)) count=0;
	} else
	{				//Zapisujemy kilka sektor�w na raz
		SD_SendCMD(sd, ACMD23, count);
		if(SD_SendCMD(sd, CMD25, sector) == 0)
		 {
			do
			{
				if (!SD_Sendblock(sd, buff, 0xfc)) break;
				buff += 512;
			} while (--count);
			if (!SD_Sendblock(sd, 0, 0xfd)) count=1;	//Token ko�ca wielobajtowego bloku danych
		}
	}
	SPI_SD_CS(sd, false);

	return count ? RES_ERROR : RES_OK;
}

