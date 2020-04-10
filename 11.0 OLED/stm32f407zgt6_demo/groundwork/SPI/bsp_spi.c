#include "bsp_spi.h"

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void mx_spi1_init(void)
{	        	
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   OLED_CLK(PA5)
//              SDA   OLED_MOSI(PA7)  
//				RST   OLED_RST(PA6)
//				DC    OLED_DC(PA4)
//				BUTTON_AD BUTTON_AD(PA3)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
//	初始化OLED_CLK(PA5)	OLED_MOSI(PA7)	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
	
	//复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);
	
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//21M
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,&SPI_InitStruct);
	SPI_Cmd(SPI1,ENABLE);
}   
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;						//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;		//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); 					//使能SPI1
} 
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
  while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
  SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据	
  while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET){}//等待接收完一个byte  
  return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据		    
}








