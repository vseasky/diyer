#ifndef __BSP_SPI_H
#define __BSP_SPI_H
#include "sys.h" 	   

void mx_spi1_init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节		 

#endif

