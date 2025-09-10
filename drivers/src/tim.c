#include "tim.h"
#include "usart.h"

// TIM6 毫秒因子和微秒因子
uint32_t tim6_fac_ms = 0;
uint32_t tim6_fac_us = 0;

/**
 * 简述：TIM6 延时初始化
 * 参数：prescaler 预分频值，取值范围 1 ~ 65535
 * 返回值：无
 **/
void tim6_init(uint16_t prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	// 使能 TIM6 时钟
	// void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	// 配置时基单元
	// TIM_TimeBaseInitStruct.TIM_ClockDivision = ;       // 只有 TIM6 和 TIM7 不使用
	// TIM_TimeBaseInitStruct.TIM_RepetitionCounter = ;   // 只 TIM1 和 TIM8 才使用
	// TIM_TimeBaseInitStruct.TIM_CounterMode = ;         // 只有 TIM6 和 TIM7 不使用，它们默认向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 0;  // 周期设置为 0
	TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler - 1;
	// void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	
	// 关闭定时器
	TIM_Cmd(TIM6, DISABLE);
	
	// 设置 TIM6 毫秒因子和微秒因子
	tim6_fac_ms = 72000 / prescaler;  // 72000000 / prescaler / 1000
	tim6_fac_us = 72 / prescaler;     // 72000000 / prescaler / 1000000
}

/**
 * 简述：TIM6 微秒延时，不考虑计数器溢出情况
 * 参数：us 延时的微秒数
 * 返回值：无
 **/
void tim6_delay_us(uint32_t us)
{

	/*设置自动重装载值*/
	TIM_SetAutoreload(TIM6, us);
	
	/*手动清除标志*/
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
	/*使能定时器*/
	TIM_Cmd(TIM6, ENABLE);
	
	/*等待更新标志*/
	while (TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) == RESET);
	
	/*失能定时器*/
	TIM_Cmd(TIM6, DISABLE);

}

/**
 * 简述：TIM6 毫秒延时，考虑计数器溢出情况
 * 参数：ms 延时的毫秒数
 * 返回值：无
 **/
void tim6_delay_ms(uint32_t ms)
{
	
	while(ms--)//5-- = 5 ;--5 = 4
	{
		tim6_delay_us(1000);
	}

}
//----------------------------------------------------------------------------------

// 使用 TIM4_CH3 控制舵机
// TIM4_CH3 - PB8 

/**
 * 简述：初始化 TIM4_CH3 用于控制 舵机
 * 参数：prescaler 预分频，取值范围 1 ~ 65535
 *       period    周期，取值范围 1 ~ 65535
 * 返回值：无
 **/
void tim4_ch3_init(uint32_t prescaler, uint32_t period)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	// 使能 GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 使能 TIM4 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	// PB8 配置为复用功能推挽输出
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// 配置 TIM4 时基单元
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;   // 只 TIM1 和 TIM8 才使用
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;  // 向上计数
	TIM_TimeBaseInitStruct.TIM_Period        = period - 1;   // 周期
	TIM_TimeBaseInitStruct.TIM_Prescaler     = prescaler - 1;  // 预分频
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;						 //配置为PWM模式1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;													 //设置占空比大小，在主函数compare中又设置一遍
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;		 //输出通道电平极性配置
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;	 //输出通道空闲电平极性配置
	TIM_OC3Init(TIM4, &TIM_OCInitStruct);			   						 //初始化通道1
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);						 //使能通道1输出
	
	// 使能定时器
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

// WIFI 1ms精准定时
void TIM3_Int_Init(void) //TIM3初始化
{

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;

	NVIC_InitTypeDef  NVIC_InitStruct;


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);


	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; 

	TIM_TimeBaseInitStruct.TIM_Period=10-1;

	TIM_TimeBaseInitStruct.TIM_Prescaler=7200-1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);


	TIM_ClearFlag(TIM3, TIM_IT_Update);

	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);//使能更新中断

//NVIC初始化
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;

	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;

	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;

	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;

	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM3, ENABLE);  //ê1?üTIMx					 

}

// syn6288的定时器


//通用定时器7中断初始化
//这里时钟选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz 
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM3时钟使能    
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	TIM_Cmd(TIM7,ENABLE);//开启定时器7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}

//定时器7中断服务程序		    
void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		USART3_RX_STA|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //清除TIM3更新中断标志    
		TIM_Cmd(TIM7, DISABLE);  //关闭TIM3 
	}	    
}


