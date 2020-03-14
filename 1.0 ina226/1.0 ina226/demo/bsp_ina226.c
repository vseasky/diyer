#include "bsp_ina226.h"
#include "delay.h"
//	接线说明：
//	模拟IIC:
//		SDA -- PF0 -- A型板 I2
//		SCL -- PF1 -- A型板 I1
INA226 ina226_data;
//初始化INA226
void mx_ina226_init(void)
{	

	IIC_Init();
	INA226_SendData(INA226_ADDR1,CFG_REG,0x8000);	//重新启动
	
	INA226_SendData(INA226_ADDR1,CFG_REG,0x484f);	//设置转换时间204us,求平均值次数128，采样时间为204*128，设置模式为分流和总线连续模式
	INA226_SendData(INA226_ADDR1,CAL_REG,CAL);	//设置分辨率
	INA226_Get_ID(INA226_ADDR1);					//获取ina226的id
}

//设置寄存器指针
void INA226_SetRegPointer(u8 addr,u8 reg)
{
	IIC_Start();

	IIC_Send_Byte(addr);
	IIC_Wait_Ack();

	IIC_Send_Byte(reg);
	IIC_Wait_Ack();

	IIC_Stop();
}

//发送,写入数据
void INA226_SendData(u8 addr,u8 reg,u16 data)
{
	u8 temp=0;
	IIC_Start();

	IIC_Send_Byte(addr);
	IIC_Wait_Ack();

	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	
	temp = (u8)(data>>8);
	IIC_Send_Byte(temp);
	IIC_Wait_Ack();

	temp = (u8)(data&0x00FF);
	IIC_Send_Byte(temp);
	IIC_Wait_Ack();
	
	IIC_Stop();
}

//读取数据
u16 INA226_ReadData(u8 addr)
{
	u16 temp=0;
	IIC_Start();

	IIC_Send_Byte(addr+1);
	IIC_Wait_Ack();
	
	temp = IIC_Read_Byte(1);
	temp<<=8;	
	temp |= IIC_Read_Byte(0);
	
	IIC_Stop();
	return temp;
}
//1mA/bit
u16 INA226_GetShunt_Current(u8 addr)
{
	u16 temp=0;	
	INA226_SetRegPointer(addr,CUR_REG);
	temp = INA226_ReadData(addr);
	if(temp&0x8000)	temp = ~(temp - 1);	
	return temp;
}

//获取id
void INA226_Get_ID(u8 addr)
{
	u32 temp=0;
	INA226_SetRegPointer(addr,INA226_GET_ADDR);
	temp = INA226_ReadData(addr);
	ina226_data.ina226_id = temp;
}

//获取校准值
u16 INA226_GET_CAL_REG(u8 addr)
{	
	u32 temp=0;
	INA226_SetRegPointer(addr,CAL_REG);
	temp = INA226_ReadData(addr);
	return (u16)temp;
}

//1.25mV/bit
u16 INA226_GetVoltage(u8 addr)
{
	u32 temp = 0;
	INA226_SetRegPointer(addr,BV_REG);
	temp = INA226_ReadData(addr);
	return (u16)temp;	
}

//2.5uV/bit
u16 INA226_GetShuntVoltage(u8 addr)
{
	int16_t temp = 0;
	INA226_SetRegPointer(addr,SV_REG);
	temp = INA226_ReadData(addr);
	if(temp&0x8000)	temp = ~(temp - 1);	
	return (u16)temp;	
}

//获取电压
void GetVoltage(float *Voltage)//mV
{
	*Voltage = INA226_GetVoltage(INA226_ADDR1)*Voltage_LSB;
}

//获取分流电压
void Get_Shunt_voltage(float *Voltage)//uV
{
	*Voltage = (INA226_GetShuntVoltage(INA226_ADDR1)*INA226_VAL_LSB);//如需矫正电流分流参数请将这里改为2.5
}

//获取电流
void Get_Shunt_Current(float *Current)//mA
{
	*Current = (INA226_GetShunt_Current(INA226_ADDR1)* CURRENT_LSB);
}

//获取功率= 总线电压 * 电流
void get_power()//W
{
	GetVoltage(&ina226_data.voltageVal);			//mV
	Get_Shunt_voltage(&ina226_data.Shunt_voltage);	//uV
	Get_Shunt_Current(&ina226_data.Shunt_Current);	//mA
	Get_Power(&ina226_data.Power);
	ina226_data.Power_Val = ina226_data.voltageVal*0.001f * ina226_data.Shunt_Current*0.001f; 	//mV*mA
}

//获取功率装载值,ina226内部计算的的功率，由于未经校准，故不采用

u16 INA226_Get_Power(u8 addr)
{
	int16_t temp=0;
	INA226_SetRegPointer(addr,PWR_REG);
	temp = INA226_ReadData(addr);
	return (u16)temp;
}

//获取功率,ina226内部计算，不准确，不采用
void Get_Power(float *Power)//W
{
	*Power = (INA226_Get_Power(INA226_ADDR1)*POWER_LSB);
}

//不设置报警，舍弃
/*
u8 INA226_AlertAddr()
{
	u8 temp;
	IIC_Start();

	IIC_Send_Byte(INA226_GETALADDR);
	IIC_Wait_Ack();
	
	temp = IIC_Read_Byte(1);
	
	IIC_Stop();
	return temp;
}
*/
