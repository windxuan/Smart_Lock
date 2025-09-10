#include "tim.h"
#include "usart.h"

// TIM6 �������Ӻ�΢������
uint32_t tim6_fac_ms = 0;
uint32_t tim6_fac_us = 0;

/**
 * ������TIM6 ��ʱ��ʼ��
 * ������prescaler Ԥ��Ƶֵ��ȡֵ��Χ 1 ~ 65535
 * ����ֵ����
 **/
void tim6_init(uint16_t prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	// ʹ�� TIM6 ʱ��
	// void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	// ����ʱ����Ԫ
	// TIM_TimeBaseInitStruct.TIM_ClockDivision = ;       // ֻ�� TIM6 �� TIM7 ��ʹ��
	// TIM_TimeBaseInitStruct.TIM_RepetitionCounter = ;   // ֻ TIM1 �� TIM8 ��ʹ��
	// TIM_TimeBaseInitStruct.TIM_CounterMode = ;         // ֻ�� TIM6 �� TIM7 ��ʹ�ã�����Ĭ�����ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 0;  // ��������Ϊ 0
	TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler - 1;
	// void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	
	// �رն�ʱ��
	TIM_Cmd(TIM6, DISABLE);
	
	// ���� TIM6 �������Ӻ�΢������
	tim6_fac_ms = 72000 / prescaler;  // 72000000 / prescaler / 1000
	tim6_fac_us = 72 / prescaler;     // 72000000 / prescaler / 1000000
}

/**
 * ������TIM6 ΢����ʱ�������Ǽ�����������
 * ������us ��ʱ��΢����
 * ����ֵ����
 **/
void tim6_delay_us(uint32_t us)
{

	/*�����Զ���װ��ֵ*/
	TIM_SetAutoreload(TIM6, us);
	
	/*�ֶ������־*/
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
	/*ʹ�ܶ�ʱ��*/
	TIM_Cmd(TIM6, ENABLE);
	
	/*�ȴ����±�־*/
	while (TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) == RESET);
	
	/*ʧ�ܶ�ʱ��*/
	TIM_Cmd(TIM6, DISABLE);

}

/**
 * ������TIM6 ������ʱ�����Ǽ�����������
 * ������ms ��ʱ�ĺ�����
 * ����ֵ����
 **/
void tim6_delay_ms(uint32_t ms)
{
	
	while(ms--)//5-- = 5 ;--5 = 4
	{
		tim6_delay_us(1000);
	}

}
//----------------------------------------------------------------------------------

// ʹ�� TIM4_CH3 ���ƶ��
// TIM4_CH3 - PB8 

/**
 * ��������ʼ�� TIM4_CH3 ���ڿ��� ���
 * ������prescaler Ԥ��Ƶ��ȡֵ��Χ 1 ~ 65535
 *       period    ���ڣ�ȡֵ��Χ 1 ~ 65535
 * ����ֵ����
 **/
void tim4_ch3_init(uint32_t prescaler, uint32_t period)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	// ʹ�� GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// ʹ�� TIM4 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	// PB8 ����Ϊ���ù����������
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// ���� TIM4 ʱ����Ԫ
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;   // ֻ TIM1 �� TIM8 ��ʹ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;  // ���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period        = period - 1;   // ����
	TIM_TimeBaseInitStruct.TIM_Prescaler     = prescaler - 1;  // Ԥ��Ƶ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;						 //����ΪPWMģʽ1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//���ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0;													 //����ռ�ձȴ�С����������compare��������һ��
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;		 //���ͨ����ƽ��������
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;	 //���ͨ�����е�ƽ��������
	TIM_OC3Init(TIM4, &TIM_OCInitStruct);			   						 //��ʼ��ͨ��1
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);						 //ʹ��ͨ��1���
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(TIM4, ENABLE);
}

void turn_pwm_3(void)
{
	TIM_SetCompare3(TIM4,1500);
}

void turn_pwm_1(void)
{
	TIM_SetCompare3(TIM4,500);
}

// WIFI 1ms��׼��ʱ
void TIM3_Int_Init(void) //TIM3��ʼ��
{

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;

	NVIC_InitTypeDef  NVIC_InitStruct;


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);


	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; 

	TIM_TimeBaseInitStruct.TIM_Period=10-1;

	TIM_TimeBaseInitStruct.TIM_Prescaler=7200-1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);


	TIM_ClearFlag(TIM3, TIM_IT_Update);

	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);//ʹ�ܸ����ж�

//NVIC��ʼ��
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;

	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;

	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;

	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;

	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM3, ENABLE);  //��1?��TIMx					 

}

// syn6288�Ķ�ʱ��


//ͨ�ö�ʱ��7�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM3ʱ��ʹ��    
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	
	TIM_Cmd(TIM7,ENABLE);//������ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}

//��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART3_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM3�����жϱ�־    
		TIM_Cmd(TIM7, DISABLE);  //�ر�TIM3 
	}	    
}


