#include "bsp_init.h"
#include "bsp_gui.h"
#include "bsp_adc.h"
#include "bmp.h"
#include "bsp_init.h"
/*	测试电压(单位: mV)
	按下 	0		0		1		0
	左 		638		638		637		637
	右		1328	1326	1324	1323
	上		1965	1968	1963	1961
	下		2599	2602	2597	2591
*/
oled_gui mx_oled_info_s;
/****************************************************************/
/*				GUI字符列表 自定义7*7种界面显示 				*/
//根据需要添加相关显示函数
u8 mx_oled_gui_str	[7][MAX_SIZE_LENGTH] = 
{".adc_text  ",".imu_task  ",".power     ",".hello4    ",".hello5    ",".hello6    ",".hello7    "};
/*adc_text*/
u8 mx_oled_gui_task1[7][MAX_SIZE_LENGTH] = 
{".adc_button",".dbus_key  ",".task1_p3  ",".task1_p4  ",".task1_p5  ",".task1_p6  ",".task1_p7  "};

u8 mx_oled_gui_task2[7][MAX_SIZE_LENGTH] = 
{".mpu6600  ",".temp_test",".task2_p3  ",".task2_p4  ",".task2_p5  ",".task2_p6  ",".task2_p7  "};

u8 mx_oled_gui_task3[7][MAX_SIZE_LENGTH] = 
{".ina226   ",".task3_p2  ",".task3_p3  ",".task3_p4  ",".task3_p5  ",".task3_p6  ",".task3_p7  "};

u8 mx_oled_gui_task4[7][MAX_SIZE_LENGTH] = 
{".task4_p1  ",".task4_p2  ",".task4_p3  ",".task4_p4  ",".task4_p5  ",".task4_p6  ",".task4_p7  "};

u8 mx_oled_gui_task5[7][MAX_SIZE_LENGTH] = 
{".task5_p1  ",".task5_p2  ",".task5_p3  ",".task5_p4  ",".task5_p5  ",".task5_p6  ",".task5_p7  "};

u8 mx_oled_gui_task6[7][MAX_SIZE_LENGTH] = 
{".task6_p1  ",".task6_p2  ",".task6_p3  ",".task6_p4  ",".task6_p5  ",".task6_p6  ",".task6_p7  "};

u8 mx_oled_gui_task7[7][MAX_SIZE_LENGTH] = 
{".task7_p1  ",".task7_p2  ",".task7_p3  ",".task7_p4  ",".task7_p5  ",".task7_p6  ",".task7_p7  "};
/*				GUI字符列表 自定义7*7种界面显示 				*/
/****************************************************************/
/*初始化gui 返回gui数据指针*/
const oled_gui* get_oled_info_data(void)
{
	
	mx_oled_info_s.oled_options_s[0] = 1;
	mx_oled_info_s.oled_options_s[1] = 1;
	mx_oled_info_s.oled_directory = 0;
	mx_oled_info_s.oled_gui_lock=0;		//默认adc按键有效，gui解锁状态
	mx_oled_info_s.olde_gui_max = MAX_GUI;
	mx_oled_info_s.oled_Interface_update = interface_main;
	return  &mx_oled_info_s;
}
/*按键处理返回adc按键值*/
u8 range_comparison(u16 get,u16 target)
{
	u16 d;
	if(target>70){d=target-70; }
	else if(target<70){d=0;}
	if(get>=d&&get<=(target+70))
	{
		return 1;
	}
	else return 0 ;
}

/*获取adc按键消息*/
void get_oled_button_ad(void)
{
	static u8 error_data = 0;
	mx_oled_info_s.adc_key_val = get_real_adc_average(BUTTON_AD_CHANNEL,10);//注意接口
	if(range_comparison(mx_oled_info_s.adc_key_val,KEY_ON)==1){mx_oled_info_s.oled_key_button_s = mx_key_on;error_data = 0;}
	if(range_comparison(mx_oled_info_s.adc_key_val,KEY_LEFT)==1){mx_oled_info_s.oled_key_button_s = mx_key_left;error_data = 0;}
	if(range_comparison(mx_oled_info_s.adc_key_val,KEY_RIGHT)==1){mx_oled_info_s.oled_key_button_s = mx_key_right;error_data = 0;}
	if(range_comparison(mx_oled_info_s.adc_key_val,KEY_TOP)==1){mx_oled_info_s.oled_key_button_s = mx_key_top;error_data = 0;}
	if(range_comparison(mx_oled_info_s.adc_key_val,KEY_BOTTOM)==1){mx_oled_info_s.oled_key_button_s = mx_key_bottom;error_data = 0;}
	if(range_comparison(mx_oled_info_s.adc_key_val,KEY_NULL)==1){mx_oled_info_s.oled_key_button_s = mx_key_null;error_data = 0;}
	else {error_data++;}
	if(error_data >=20)
	{	
		mx_oled_info_s.oled_key_button_s = mx_key_null;
//		printf("key_button_ad:error!");//报告adc的值不在范围内
	}
}

/*gui界面消息处理 界面更新信息处理*/
void interface_main(void)
{
	get_oled_button_ad();//获取adc按键消息
	switch(mx_oled_info_s.oled_key_button_s)
	{
		case mx_key_null   :break;
		case mx_key_on     :break;
		case mx_key_left   : 
		{
			if(mx_oled_info_s.oled_gui_lock==0)
			if(mx_oled_info_s.oled_directory>0)
			{
				mx_oled_info_s.oled_directory--;
				OLED_Clear();//更换界面清屏
			}
		}break;
		case mx_key_right  :
		{
			if(mx_oled_info_s.oled_gui_lock==0)
			if(mx_oled_info_s.oled_directory<MAX_D)
			{
				mx_oled_info_s.oled_directory++;
				OLED_Clear();//更换界面清屏
			}
		}
		break;
		case mx_key_top    :
			{
				if(mx_oled_info_s.oled_directory==0)break;//当前在0级界面跳出
				if(mx_oled_info_s.oled_directory==3)break;//当前在3级界面跳出
				if(mx_oled_info_s.oled_options_s[mx_oled_info_s.oled_directory-1]>1)
				{mx_oled_info_s.oled_options_s[mx_oled_info_s.oled_directory-1]--;}
			}
			break;
		case mx_key_bottom :
			{
				if(mx_oled_info_s.oled_directory==0)break;
				if(mx_oled_info_s.oled_directory==3)break;
				if(mx_oled_info_s.oled_options_s[mx_oled_info_s.oled_directory-1]<mx_oled_info_s.olde_gui_max)
				{mx_oled_info_s.oled_options_s[mx_oled_info_s.oled_directory-1]++;}
			}break;	
	}		
	switch(mx_oled_info_s.oled_directory)
	{
		case 0:{OLED_DrawBMP(0,0,128,6,BMP2);mx_oled_info_s.oled_options_s[0]=1;};break;
		case 1:{interface_finger(mx_oled_info_s.oled_options_s[0],
								mx_oled_gui_str);
				mx_oled_info_s.oled_options_s[1]=1;
			  };break;
		case 2:interface_second();break;
		case 3:interface_second();break;
	}
}

/*gui项目界面显示函数*/
void interface_second(void)
{
	switch(mx_oled_info_s.oled_options_s[0])
	{
		case 1:interface_task1();break;
		case 2:interface_task2();break;
		case 3:interface_task3();break;
		case 4:interface_task4();break;
		case 5:interface_task5();break;
		case 6:interface_task6();break;
		case 7:interface_task7();break;
	}
}

void interface_finger(u8 get_finger,u8 fin_mx_task[7][MAX_SIZE_LENGTH])
{
	u8 k;
	static u8 min=1,max=4;
	if(get_finger>max)
	{
		min=get_finger-3;
		max = get_finger;
	}
	if(get_finger<min)
	{
		min=get_finger;
		max = get_finger+3;
	}
	for(k=0;k<4;k++)
	{
		if(min+k==get_finger)OLED_ShowChar(0,(k)*2,'-');
		else OLED_ShowChar(0,(k)*2,' ');
		OLED_ShowNum(10,(k)*2,min+k,1,16);
		OLED_ShowString(20,(k)*2,&fin_mx_task[min+k-1][0]);
	}
}

/*gui界面函数1*/
void interface_task1(void)
{
	if(mx_oled_info_s.oled_directory==2)
				interface_finger(mx_oled_info_s.oled_options_s[1],					
				mx_oled_gui_task1);
	if(mx_oled_info_s.oled_directory==3)
	{
		switch(mx_oled_info_s.oled_options_s[1])
		{
			case 1:task1_adc_button();break;
			case 2:dbus_show();break;
			case 3:;break;
			case 4:;break;
			case 5:;break;
			case 6:;break;
			case 7:;break;
		}
	}
}
/*gui界面函数2*/
void interface_task2(void)
{
	if(mx_oled_info_s.oled_directory==2)
		interface_finger(mx_oled_info_s.oled_options_s[1],					
				mx_oled_gui_task2);
	if(mx_oled_info_s.oled_directory==3)
	{
		switch(mx_oled_info_s.oled_options_s[1])
		{
			case 1:task2_imu();break;
			case 2:task2_temp();break;
			case 3:;break;
			case 4:;break;
			case 5:;break;
			case 6:;break;
			case 7:;break;
		}
	}
}
/*gui界面函数3*/
void interface_task3(void)
{
	if(mx_oled_info_s.oled_directory==2)
		interface_finger(mx_oled_info_s.oled_options_s[1],					
				mx_oled_gui_task3);
	if(mx_oled_info_s.oled_directory==3)
	{
		switch(mx_oled_info_s.oled_options_s[1])
		{
			case 1:ina226_task3();break;
			case 2:task2_temp();break;
			case 3:;break;
			case 4:;break;
			case 5:;break;
			case 6:;break;
			case 7:;break;
		}
	}
}
/*gui界面函数4*/
void interface_task4(void)
{
	interface_finger(mx_oled_info_s.oled_options_s[1],					
				mx_oled_gui_task4);
}
/*gui界面函数5*/
void interface_task5(void)
{
	interface_finger(mx_oled_info_s.oled_options_s[1],					
				mx_oled_gui_task5);
}
/*gui界面函数6*/
void interface_task6(void)
{
	interface_finger(mx_oled_info_s.oled_options_s[1],					
				mx_oled_gui_task6);
}
/*gui界面函数7*/
void interface_task7(void)
{
	interface_finger(mx_oled_info_s.oled_options_s[1],					
				mx_oled_gui_task7);
}

/*adc_button测试显示函数*/
void task1_adc_button(void)
{
	gui_lock();//因为是adc按键测试，故锁定ui界面
	OLED_ShowString(2,0,(u8*)"button_ad_text");
	OLED_ShowNum(10,4,mx_oled_info_s.adc_key_val,7,16);
	OLED_ShowString(80,4,(u8*)"mV");
	//由于adc按键测试锁定了gui界面需手动解锁
	gui_unlock_s();
}
void dbus_show(void)
{
	OLED_ShowString(2,0,(u8*)"ch0");
	OLED_ShowNum(30,0,mx_info_sum.rc_crrl_info_s->rc.ch0,7,16);
	
	OLED_ShowNum(0,2,mx_info_sum.rc_crrl_info_s->mouse.x,7,16);
	

	OLED_ShowNum(0,6,mx_info_sum.rc_crrl_info_s->mouse.y,7,16);
	

	OLED_ShowNum(0,8,mx_info_sum.rc_crrl_info_s->keyboard.key_code,7,16);
}
/*imu测试显示函数*/
void task2_imu(void)
{
	u32 dat;
	OLED_ShowString(2,0,(u8*)"imu_test: /1000");
	OLED_ShowString(2,2,(u8*)"pitch:");
	OLED_ShowString(2,4,(u8*)"roll:");
	OLED_ShowString(2,6,(u8*)"yaw:");
	dat = (u32)mx_info_sum.imu_info_s->pitch*1000.0;
	OLED_ShowNum(70,2,dat,7,16);
	dat = (u32)mx_info_sum.imu_info_s->roll*1000.0;
	OLED_ShowNum(70,4,dat,7,16);
	dat = (u32)mx_info_sum.imu_info_s->yaw*1000.0;
	OLED_ShowNum(70,6,dat,7,16);
}
void task2_temp(void)
{
	u32 dat;
	OLED_ShowString(2,0,(u8*)"temp_test");
	OLED_ShowString(2,2,(u8*)"cpu_temp:");
	OLED_ShowString(2,4,(u8*)"imu_temp:");
	dat =(u32) *mx_info_sum.mx_cpu_temp;
	OLED_ShowNum(70,2,dat,6,16);
	dat = (u32)mx_info_sum.imu_info_s->temp;
	OLED_ShowNum(70,4,dat,6,16);
}


void ina226_task3(void)
{
	u32 dat;
	OLED_ShowString(2,0,(u8*)"RoboMaster");
	OLED_ShowString(2,2,(u8*)"Voltage:");
	OLED_ShowString(2,4,(u8*)"Current:");
	OLED_ShowString(2,6,(u8*)"Power  :");
	
	dat =(u32) ina226_data.voltageVal/1000.0f;
	OLED_ShowNum(69,2,dat,5,16);
	dat =(u32) ina226_data.Shunt_Current/1000.0f;
	OLED_ShowNum(69,4,dat,5,16);
	dat =(u32) ina226_data.Power/1000.0f;
	OLED_ShowNum(69,6,dat,5,16);
	
	OLED_ShowString(115,2,(u8*)"V");
	OLED_ShowString(115,4,(u8*)"A");
	OLED_ShowString(115,6,(u8*)"W");
}
//锁定ui界面
void gui_lock(void)
{
	mx_oled_info_s.oled_gui_lock = 1;
}
//gui界面手动解锁
void gui_unlock_s(void)
{
	if(mx_info_sum.key_info_s->key_info.states != KEY_UPSPRING)
	{
		OLED_Clear();
		mx_oled_info_s.oled_gui_lock = 0;
		mx_oled_info_s.oled_directory--;
	}
}

