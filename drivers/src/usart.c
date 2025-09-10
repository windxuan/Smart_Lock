#include "usart.h"
#include "stm32f10x.h"
#include "tim.h"
#include "stdio.h"

// WIFI ����
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

// usb�������
// ���� USART1 ���ջ�����
volatile uint8_t usart1_recv_buf[USART1_RECV_BUF_SIZE] = "";
volatile int usart1_recv_index = 0;
volatile int usart1_get_index = 0;
volatile int usart1_idle_flag = 0;

/**
 * ��������ʼ�� USART1��ʹ���жϽ���
 * ������baudrate ������
 * ����ֵ����
 **/
void usart1_init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// ʹ�� GPIOA �� USART1 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	// ���� PA9 Ϊ���ù����������
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// ���� PA10 Ϊ��������
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// ���� USART1 ����λ����żУ��λ��ֹͣλ��Ӳ�����ء������ʵ�
	USART_InitStruct.USART_BaudRate            = baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity              = USART_Parity_No;
	USART_InitStruct.USART_StopBits            = USART_StopBits_1;
	USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
	// void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
	USART_Init(USART1, &USART_InitStruct);
	
	// NVIC ����
	NVIC_InitStruct.NVIC_IRQChannel                   = USART1_IRQn;  // ͨ����
	NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 2;
	// void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct);
	NVIC_Init(&NVIC_InitStruct);
	
	// ���ý��շǿ��жϺͿ����ж�
	// void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	
	// ʹ�� USART1
	// void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
	USART_Cmd(USART1, ENABLE);
}


/**
 * ������USART ���Ͷ��ֽں���
 * ������@USARTx:ѡ��ʹ�õĴ���
				@data:Ҫ���͵����ݵĵ�ַ
				@len��Ҫ���͵����ݵĳ���
 * ����ֵ����
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
 * ������USART1 ��������
 * ��������
 * ����ֵ���յ�������
 **/
int usart1_getchar(void)
{
	int data;
	
	// �ȴ�����
	while (usart1_get_index >= usart1_recv_index);
	
	// ��������
	data = usart1_recv_buf[usart1_get_index];
	usart1_get_index++;
	if (usart1_get_index >= usart1_recv_index) {
		usart1_get_index = usart1_recv_index = usart1_idle_flag = 0;
	}
	
	// ����
	usart1_putchar(data);
	
	return data;  // �����յ�������
}

int usart1_putchar(int data)
{
	// �ȴ��ɷ������ݣ������Ϳձ�־ TXE��
	// FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	// ��������
	// void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
	USART_SendData(USART1, data);
	
	return data;
}

//int fputc(int data)  // �˺����� printf() �ȱ�׼�����������
//{
//	return usart1_putchar(data);
//}

//int fgetc(void)  // �˺����� scanf() �ȱ�׼���뺯������
//{
//	return usart1_getchar();
//}

#if 0  // ����
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
	while((USART3->SR & 0X40) == 0); //ѭ������,ֱ���������
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

#if 0 // ����
#if EN_USART3_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
u16 USART3_RX_STA = 0;     //����״̬���

void USART3_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//USART3_TX   GPIOC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	//USART3_RX	  GPIOC.11��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	//USART3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1

	TIM7_Int_Init(99, 7199);		//10ms�ж�
	USART3_RX_STA = 0;		//����
	TIM_Cmd(TIM7, DISABLE);			//�رն�ʱ��7
}

void USART3_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART3);	//��ȡ���յ�������
		if((USART3_RX_STA & 0x8000) == 0) //����δ���
		{
			if(USART3_RX_STA < USART3_REC_LEN)	//�����Խ�������
			{
				TIM_SetCounter(TIM7, 0); //���������          				//���������
				if(USART3_RX_STA == 0) 				//ʹ�ܶ�ʱ��7���ж�
				{
					TIM_Cmd(TIM7, ENABLE); //ʹ�ܶ�ʱ��7
				}
				USART3_RX_BUF[USART3_RX_STA++] = Res;	//��¼���յ���ֵ
			}
			else
			{
				USART3_RX_STA |= 1 << 15;				//ǿ�Ʊ�ǽ������
			}
		}
	}
}
#endif
#endif // ����



#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
	while((USART3->SR & 0X40) == 0); //ѭ������,ֱ���������
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

#if EN_USART3_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
u16 USART3_RX_STA = 0;     //����״̬���

void USART3_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//USART3_TX   GPIOC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	//USART3_RX	  GPIOC.11��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	//USART3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1

	TIM7_Int_Init(99, 7199);		//10ms�ж�
	USART3_RX_STA = 0;		//����
	TIM_Cmd(TIM7, DISABLE);			//�رն�ʱ��7
}

void USART3_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART3);	//��ȡ���յ�������
		if((USART3_RX_STA & 0x8000) == 0) //����δ���
		{
			if(USART3_RX_STA < USART3_REC_LEN)	//�����Խ�������
			{
				TIM_SetCounter(TIM7, 0); //���������          				//���������
				if(USART3_RX_STA == 0) 				//ʹ�ܶ�ʱ��7���ж�
				{
					TIM_Cmd(TIM7, ENABLE); //ʹ�ܶ�ʱ��7
				}
				USART3_RX_BUF[USART3_RX_STA++] = Res;	//��¼���յ���ֵ
			}
			else
			{
				USART3_RX_STA |= 1 << 15;				//ǿ�Ʊ�ǽ������
			}
		}
	}
}
#endif