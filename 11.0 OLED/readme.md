开源一款OLED

<img src="./image/OLED.JPG" width="800"  />
<img src="./image/IMG_5240.JPG" width="800"  />
<img src="./image/IMG_5241.JPG" width="800"  />


此OLED完美兼容大疆OLED，可以直接接在RoboMaster开发板上使用

附上使用视频：[https://b23.tv/BV1mV411f7mR](https://b23.tv/BV1mV411f7mR "bilibili_OLED效果演示")

该项目包含：

1.0完整的AD工程文档。

2.0基于STM32F407+Keil5的完整演示程序，当然程序兼容部分STM32F4系列单片机，只需要IO相同即可。

3.0OLED调试工具

在使用之前务必初始化底层硬件接口，并映射到自己的开发板

如果你没有使用过FreeRTOS
请在初始化完成后紧接着添加如下代码

	OLED_Clear();
	while(1)
	{
		interface_main();	//更新oled	
		delay(100);
	}

如需技术交流请进SEASKY开源技术交流群:893445794
