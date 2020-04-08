#include "start_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "usart.h"
#include "bsp_gui.h"
#include "bsp_oled.h"


#define START_TASK_PRIO 1
#define START_TASK_STK_SIZE 128
TaskHandle_t Start_Task_Handler;

#define LED0_TASK_PRIO 2
#define LED0_TASK_STK_SIZE 128
TaskHandle_t Led0_Task_Handler;

#define LED1_TASK_PRIO 3
#define LED1_TASK_STK_SIZE 128
TaskHandle_t Led1_Task_Handler;

void create_start_task(void)
{
	xTaskCreate((TaskFunction_t)start_task,
				(char*         )"start_task",
				(uint16_t      )START_TASK_STK_SIZE,
				(void *        )NULL,
				(UBaseType_t   )START_TASK_PRIO,
				(TaskHandle_t*)&Start_Task_Handler);
}
void led0_task(void *pvParameters)
{
	OLED_Clear();
	while(1)
	{
		interface_main();	//¸üÐÂoled	
		vTaskDelay(100);
	}
}
void led1_task(void *pvParameters)
{
	while(1)
	{
		LED0 =~LED0;
		LED1 =~LED1;
		vTaskDelay(500);
	}
}
void start_task(void *pvParameters)
{
	xTaskCreate((TaskFunction_t)led0_task,
				(char*         )"led0_task",
				(uint16_t      )LED0_TASK_STK_SIZE,
				(void *        )NULL,
				(UBaseType_t   )LED0_TASK_PRIO,
				(TaskHandle_t*)&Led0_Task_Handler);
	xTaskCreate((TaskFunction_t)led1_task,
				(char*         )"led1_task",
				(uint16_t      )LED1_TASK_STK_SIZE,
				(void *        )NULL,
				(UBaseType_t   )LED1_TASK_PRIO,
				(TaskHandle_t*)&Led1_Task_Handler);
	vTaskDelete(Start_Task_Handler);
}

