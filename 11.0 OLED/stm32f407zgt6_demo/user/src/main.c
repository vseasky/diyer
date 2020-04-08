#include "FreeRTOS.h"
#include "task.h"

#include "start_task.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "bsp_oled.h"
#include "bsp_spi.h"
#include "bsp_adc.h"
#include "bsp_gui.h"
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   OLED_CLK(PA5)
//              SDA   OLED_MOSI(PA7)  
//				RST   OLED_RST(PA6)
//				DC    OLED_DC(PA4)
//				BUTTON_AD BUTTON_AD(PA3)
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);		//初始化延时函数
	uart_init(115200);     	//初始化串口
	LED_Init();		        //初始化LED端口
	mx_adc_init();
	mx_spi1_init();
	mx_oled_init();
	oled_gui_init();
	create_start_task();	//创建开始任务            
    vTaskStartScheduler();  //开启任务调度
	while(1){}
}





