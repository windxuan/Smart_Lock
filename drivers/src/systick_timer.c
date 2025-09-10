#include "systick_timer.h"

u32 fu_Nms;
u32  fu_Nus;

void systick_config(u8 sysclk)
{
	SysTick->CTRL   &=~0x05;   //设置时钟源8分频，并且在初始化程序中关闭定时器
	fu_Nus=sysclk/8;           //* 1us所需要的计的次数
	fu_Nms=fu_Nus*1000;			//* 1ms所需要的计的次数
}


void systick_Nms(int Nms)
{
	uint32_t temp;   //定义一个内存单元用来放SysTick->CTRL的值
	uint8_t  systickflag=0;//定义一个标志位，用来判断是否计数完成
	while(systickflag==0)
	{
		if(Nms<=1000)//如果NMS>1000,代表要进行多次定时，如果小于1000，代表定时只有一次，不会超出最大值
		{
			systickflag=1;  //计时剩下最后一次，定时完成后返回调用处
			SysTick->LOAD  =fu_Nms*Nms; //导入计数值，计数值等于1ms所需要的计的次数*Nms
		}
		else
		{
			Nms=Nms-1000; //大于1000时，以1000为单位定时。
			SysTick->LOAD  =fu_Nms*1000; //导入计数值，计数值等于1ms所需要的计的次数*Nms
		}
		SysTick->VAL   =0;    //清除当前值，在用到当前值的时候需要清除
		SysTick->CTRL  |=0x01;  //准备完成，开始定时，打开定时器
		do
		{
		temp=SysTick->CTRL;	//将SysTick->CTRL的值给temp，读取将SysTick->CTRL的第16位
		}while(!(temp&(1<<16))&&(temp&0x01));//判断SysTick->CTRL的第16位是否为1，为1代表计数完成，否则继续等待，并且SysTick->CTRL在正常开启的情况下
		SysTick->CTRL  &=~0x01;//计数完成关闭定时器
		SysTick->VAL   =0; //清除当前值，在用到当前值的时候需要清除
	}
}

void SysTicks_NusDelay(uint32_t Nus)
{
	uint32_t temp;   //定义一个内存单元用来放SysTick->CTRL的值
	SysTick->LOAD  =fu_Nus*Nus; //导入计数值，计数值等于1ms所需要的计的次数*Nms
	SysTick->VAL   =0;    //清除当前值，在用到当前值的时候需要清除
	SysTick->CTRL  |=0x01;  //准备完成，开始定时，打开定时器
	do
	{
	temp=SysTick->CTRL;	//将SysTick->CTRL的值给temp，读取将SysTick->CTRL的第16位
	}while(!(temp&(1<<16))&&(temp&0x01));//判断SysTick->CTRL的第16位是否为1，为1代表计数完成，否则继续等待，并且SysTick->CTRL在正常开启的情况下
	SysTick->CTRL  &=~0x01;//计数完成关闭定时器
	SysTick->VAL   =0; //清除当前值，在用到当前值的时候需要清除
}

