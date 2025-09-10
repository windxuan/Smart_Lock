#include "usart.h"
#include "stm32f10x.h"
#include "tim.h"
#include "stdio.h"

// WIFI 传输
void USART2_Config(void)
{

	 GPIO_InitTypeDef GPIO_InitStruct;

	 USART_InitTypeDef USART_InitStruct;

	 NVIC_InitTypeDef NVIC_InitStruct;



	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	GPIO_InitStruct.GPIO_Mode    =GPIO_Mode_AF_PP;

	GPIO_InitStruct.GPIO_Pin     =GPIO_Pin_2;

	GPIO_InitStruct.GPIO_Speed    =GPIO_Speed_10MHz;

	GPIO_Init(GPIOA,& GPIO_InitStruct); 

 
	GPIO_InitStruct.GPIO_Mode    =GPIO_Mode_IN_FLOATING;

	GPIO_InitStruct.GPIO_Pin     =GPIO_Pin_3;

	GPIO_Init(GPIOA,& GPIO_InitStruct);

 
	USART_InitStruct.USART_BaudRate =9600;

	USART_InitStruct.USART_HardwareFlowControl  =USART_HardwareFlowControl_None;

	USART_InitStruct.USART_Mode          =USART_Mode_Rx|USART_Mode_Tx;

	USART_InitStruct.USART_Parity         =USART_Parity_No;

	USART_InitStruct.USART_StopBits      	 =USART_StopBits_1;

	USART_InitStruct.USART_WordLength       =USART_WordLength_8b;

	USART_Init(USART2, &USART_InitStruct);


	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
 

	NVIC_InitStruct.NVIC_IRQChannel     =USART2_IRQn;

	NVIC_InitStruct.NVIC_IRQChannelCmd    =ENABLE;

	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   =0;

	NVIC_InitStruct.NVIC_IRQChannelSubPriority      =3;

	NVIC_Init(&NVIC_InitStruct);


	USART_ClearFlag(USART2,USART_FLAG_TXE);

	USART_Cmd(USART2,ENABLE);

}

// usb串口输出
// 定义 USART1 接收缓冲区
volatile uint8_t usart1_recv_buf[USART1_RECV_BUF_SIZE] = "";
volatile int usart1_recv_index = 0;
volatile int usart1_get_index = 0;
volatile int usart1_idle_flag = 0;

/**
 * 简述：初始化 USART1，使用中断接收
 * 参数：baudrate 波特率
 * 返回值：无
 **/
void usart1_init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// 使能 GPIOA 和 USART1 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	// 配置 PA9 为复用功能推挽输出
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 配置 PA10 为上拉输入
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 配置 USART1 数据位、奇偶校验位、停止位、硬件流控、波特率等
	USART_InitStruct.USART_BaudRate            = baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity              = USART_Parity_No;
	USART_InitStruct.USART_StopBits            = USART_StopBits_1;
	USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
	// void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
	USART_Init(USART1, &USART_InitStruct);
	
	// NVIC 配置
	NVIC_InitStruct.NVIC_IRQChannel                   = USART1_IRQn;  // 通道号
	NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 2;
	// void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct);
	NVIC_Init(&NVIC_InitStruct);
	
	// 设置接收非空中断和空闲中断
	// void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	
	// 使能 USART1
	// void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
	USART_Cmd(USART1, ENABLE);
}


/**
 * 简述：USART 发送多字节函数
 * 参数：@USARTx:选择使用的串口
				@data:要发送的数据的地址
				@len：要发送的数据的长度
 * 返回值：无
 **/
void USARTx_SendNByte(USART_TypeDef* USARTx ,uint8_t *data, uint8_t len)
{
	uint32_t i;
	for(i = 0;i < len ;i++)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, data[i]);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}

}



/**
 * 简述：USART1 接收数据
 * 参数：无
 * 返回值：收到的数据
 **/
int usart1_getchar(void)
{
	int data;
	
	// 等待数据
	while (usart1_get_index >= usart1_recv_index);
	
	// 接收数据
	data = usart1_recv_buf[usart1_get_index];
	usart1_get_index++;
	if (usart1_get_index >= usart1_recv_index) {
		usart1_get_index = usart1_recv_index = usart1_idle_flag = 0;
	}
	
	// 回显
	usart1_putchar(data);
	
	return data;  // 返回收到的数据
}

int usart1_putchar(int data)
{
	// 等待可发送数据（即发送空标志 TXE）
	// FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	// 发送数据
	// void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
	USART_SendData(USART1, data);
	
	return data;
}

//int fputc(int data)  // 此函数由 printf() 等标准输出函数调用
//{
//	return usart1_putchar(data);
//}

//int fgetc(void)  // 此函数由 scanf() 等标准输入函数调用
//{
//	return usart1_getchar();
//}

#if 0  // 喇叭
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
	while((USART3->SR & 0X40) == 0); //循环发送,直到发送完毕
	USART3->DR = (u8) ch;
	return ch;
}
#endif


//void USART3_SendData(u8 data)
//{
//	while((USART3->SR & 0X40) == 0);
//	USART3->DR = data;
//}

//void USART3_SendString(u8 *DAT, u8 len)
//{
//	u8 i;
//	for(i = 0; i < len; i++)
//	{
//		USART3_SendData(*DAT++);
//	}
//}

#if 0 // 喇叭
#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
u16 USART3_RX_STA = 0;     //接收状态标记

void USART3_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//USART3_TX   GPIOC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

	//USART3_RX	  GPIOC.11初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

	//USART3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    //使能串口1

	TIM7_Int_Init(99, 7199);		//10ms中断
	USART3_RX_STA = 0;		//清零
	TIM_Cmd(TIM7, DISABLE);			//关闭定时器7
}

void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART3);	//读取接收到的数据
		if((USART3_RX_STA & 0x8000) == 0) //接收未完成
		{
			if(USART3_RX_STA < USART3_REC_LEN)	//还可以接收数据
			{
				TIM_SetCounter(TIM7, 0); //计数器清空          				//计数器清空
				if(USART3_RX_STA == 0) 				//使能定时器7的中断
				{
					TIM_Cmd(TIM7, ENABLE); //使能定时器7
				}
				USART3_RX_BUF[USART3_RX_STA++] = Res;	//记录接收到的值
			}
			else
			{
				USART3_RX_STA |= 1 << 15;				//强制标记接收完成
			}
		}
	}
}
#endif
#endif // 喇叭



#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
	while((USART3->SR & 0X40) == 0); //循环发送,直到发送完毕
	USART3->DR = (u8) ch;
	return ch;
}
#endif

void USART3_SendData(u8 data)
{
	while((USART3->SR & 0X40) == 0);
	USART3->DR = data;
}

void USART3_SendString(u8 *DAT, u8 len)
{
	u8 i;
	for(i = 0; i < len; i++)
	{
		USART3_SendData(*DAT++);
	}
}

#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
u16 USART3_RX_STA = 0;     //接收状态标记

void USART3_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//USART3_TX   GPIOC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

	//USART3_RX	  GPIOC.11初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

	//USART3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    //使能串口1

	TIM7_Int_Init(99, 7199);		//10ms中断
	USART3_RX_STA = 0;		//清零
	TIM_Cmd(TIM7, DISABLE);			//关闭定时器7
}

void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART3);	//读取接收到的数据
		if((USART3_RX_STA & 0x8000) == 0) //接收未完成
		{
			if(USART3_RX_STA < USART3_REC_LEN)	//还可以接收数据
			{
				TIM_SetCounter(TIM7, 0); //计数器清空          				//计数器清空
				if(USART3_RX_STA == 0) 				//使能定时器7的中断
				{
					TIM_Cmd(TIM7, ENABLE); //使能定时器7
				}
				USART3_RX_BUF[USART3_RX_STA++] = Res;	//记录接收到的值
			}
			else
			{
				USART3_RX_STA |= 1 << 15;				//强制标记接收完成
			}
		}
	}
}
#endif