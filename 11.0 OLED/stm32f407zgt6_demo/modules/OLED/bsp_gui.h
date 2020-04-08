#ifndef _BSP_GUI_H
#define _BSP_GUI_H
#include "bsp_adc.h"
/*	测试电压(单位: mV)
	按下 	0		0		1		0
	左 		638		638		637		637
	右		1328	1326	1324	1323
	上		1965	1968	1963	1961
	下		2599	2602	2597	2591
*/
#define KEY_NULL  3240
#define KEY_ON  0
#define KEY_LEFT 637
#define KEY_RIGHT 1325
#define KEY_TOP 1965
#define KEY_BOTTOM 2599
#define BUTTON_AD_CHANNEL ADC_Channel_3

#define MAX_GUI 7
#define MAX_D 3 //最大目录三层
#define MAX_SIZE_LENGTH 12
typedef enum
{
	oled_interface_task1 = 0,
	oled_interface_task2  = 1,
	oled_interface_task3 = 2,
	oled_interface_task4 = 3,
	oled_interface_task5 =4,
	oled_interface_task6 = 5,
	oled_interface_task7 = 6,
}oled_gui_task;
typedef enum
{
	mx_key_null = 0,
	mx_key_on  = 1,
	mx_key_left = 2,
	mx_key_right = 3,
	mx_key_top =4,
	mx_key_bottom = 5,
}oled_key_button;

typedef struct
{
	u8 oled_gui_lock:2;	 //该值为0,gui可更改，该值为1，gui锁定
	u8 oled_options_s[2];//定义项目标签 默认项目一
	u8 oled_directory;   //定义目录标签 默认0级目录
	u8 olde_gui_max;
	u16 adc_key_val;
	oled_gui_task oled_gui_tsak_s;
	oled_key_button oled_key_button_s;
	void (*oled_Interface_update)(void);
}oled_gui;

const oled_gui* get_oled_info_data(void);
void get_oled_button_ad(void);
void interface_main(void);
void interface_second(void);
void interface_finger(u8 i,u8 fin_mx_task[7][MAX_SIZE_LENGTH]);
void interface_task1(void);
void interface_task2(void);
void interface_task3(void);
void interface_task4(void);
void interface_task5(void);
void interface_task6(void);
void interface_task7(void);


void oled_gui_init(void);
void task1_adc_button(void);
void dbus_show(void);
void ina226_task3(void);
void task2_imu(void);
void task2_temp(void);

void gui_lock(void);	 //gui锁定函数
void gui_unlock_s(void); //gui解锁函数，根据实际情况修改触发条件
u8 range_comparison(u16 get,u16 target);
#endif
