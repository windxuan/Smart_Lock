#include "systick_timer.h"

u32 fu_Nms;
u32  fu_Nus;

void systick_config(u8 sysclk)
{
	SysTick->CTRL   &=~0x05;   //����ʱ��Դ8��Ƶ�������ڳ�ʼ�������йرն�ʱ��
	fu_Nus=sysclk/8;           //* 1us����Ҫ�ļƵĴ���
	fu_Nms=fu_Nus*1000;			//* 1ms����Ҫ�ļƵĴ���
}


void systick_Nms(int Nms)
{
	uint32_t temp;   //����һ���ڴ浥Ԫ������SysTick->CTRL��ֵ
	uint8_t  systickflag=0;//����һ����־λ�������ж��Ƿ�������
	while(systickflag==0)
	{
		if(Nms<=1000)//���NMS>1000,����Ҫ���ж�ζ�ʱ�����С��1000������ʱֻ��һ�Σ����ᳬ�����ֵ
		{
			systickflag=1;  //��ʱʣ�����һ�Σ���ʱ��ɺ󷵻ص��ô�
			SysTick->LOAD  =fu_Nms*Nms; //�������ֵ������ֵ����1ms����Ҫ�ļƵĴ���*Nms
		}
		else
		{
			Nms=Nms-1000; //����1000ʱ����1000Ϊ��λ��ʱ��
			SysTick->LOAD  =fu_Nms*1000; //�������ֵ������ֵ����1ms����Ҫ�ļƵĴ���*Nms
		}
		SysTick->VAL   =0;    //�����ǰֵ�����õ���ǰֵ��ʱ����Ҫ���
		SysTick->CTRL  |=0x01;  //׼����ɣ���ʼ��ʱ���򿪶�ʱ��
		do
		{
		temp=SysTick->CTRL;	//��SysTick->CTRL��ֵ��temp����ȡ��SysTick->CTRL�ĵ�16λ
		}while(!(temp&(1<<16))&&(temp&0x01));//�ж�SysTick->CTRL�ĵ�16λ�Ƿ�Ϊ1��Ϊ1���������ɣ���������ȴ�������SysTick->CTRL�����������������
		SysTick->CTRL  &=~0x01;//������ɹرն�ʱ��
		SysTick->VAL   =0; //�����ǰֵ�����õ���ǰֵ��ʱ����Ҫ���
	}
}

void SysTicks_NusDelay(uint32_t Nus)
{
	uint32_t temp;   //����һ���ڴ浥Ԫ������SysTick->CTRL��ֵ
	SysTick->LOAD  =fu_Nus*Nus; //�������ֵ������ֵ����1ms����Ҫ�ļƵĴ���*Nms
	SysTick->VAL   =0;    //�����ǰֵ�����õ���ǰֵ��ʱ����Ҫ���
	SysTick->CTRL  |=0x01;  //׼����ɣ���ʼ��ʱ���򿪶�ʱ��
	do
	{
	temp=SysTick->CTRL;	//��SysTick->CTRL��ֵ��temp����ȡ��SysTick->CTRL�ĵ�16λ
	}while(!(temp&(1<<16))&&(temp&0x01));//�ж�SysTick->CTRL�ĵ�16λ�Ƿ�Ϊ1��Ϊ1���������ɣ���������ȴ�������SysTick->CTRL�����������������
	SysTick->CTRL  &=~0x01;//������ɹرն�ʱ��
	SysTick->VAL   =0; //�����ǰֵ�����õ���ǰֵ��ʱ����Ҫ���
}

