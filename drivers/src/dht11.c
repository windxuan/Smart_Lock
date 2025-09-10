#include "dht11.h"

// �鿴ԭ��ͼ�ã�
// DHT11 �� PA8 ����

// ���ݴ��
volatile unsigned int data[4];

/**
 * ���� �� DHT11 ��������
 * ���� �� ��
 * ����ֵ �� ��
 **/
 
void dhtll_init_in(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// �� GPIOA ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE );
	
	// ���� PA8
	//void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

/**
 * ���� �� DHT11 �������
 * ���� �� ��
 * ����ֵ �� ��
 **/
 
void dhtll_init_out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// �� GPIOA ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE );
	
	// ���� PA8
	//void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

/**
 * ���� ���������Ϳ�ʼ�ź�
 * ���� �� ��
 * ����ֵ �� ��
 **/
void DHT11_start(void)
{
	// ���ģʽ
	dhtll_init_out();
	
	// �����ߵ�ƽ
	dht11_high();
	tim6_delay_ms(30);
	
	// ���͵�ƽ
	dht11_low();
	tim6_delay_ms(18);
	
	// ���ߵ�ƽ
	dht11_high();
	tim6_delay_us(30);
	
	// ����ģʽ
	dhtll_init_in();
}

/**
 * ���� �� ��ȡһ���ֽ�
 * ���� �� ��
 * ����ֵ �� ��ȡ�����ֽ�
 **/
char DHT11_RBYTE(void)
{
	unsigned int i = 0;
	unsigned int data;
	
	for (i = 0; i < 8; i++)
	{
		// �ȴ��źű�ɸߵ�ƽ
		while (dht11_data == 0);
		
		// ��ʱ30us
		tim6_delay_us(30);
		
		// data����һλ
		data <<= 1;
		
		if(dht11_data == 1)
		{
			data |= 1;
		}
		
		// �ȴ��͵�ƽ����
		while (dht11_data == 1);
	}
	return data;
}

/**
 * ���� �� ��ȡ��ʪ�ȵ�������Ϣ
 * ���� �� ��
 * ����ֵ �� ��
 **/
void GET_DATA(void)
{
	unsigned char RH_H, RH_L, C_H, C_L, CHECK;
	unsigned int RHH, RHL, CH, CL;
	
	// �������Ϳ�ʼ�ź�
	DHT11_start();
	
	// �ж�DH11T�Ļ�Ӧ�ź�, 0 - ��Ӧ  1 - δ��Ӧ
	if (dht11_data == 0)
	{
		// �ȴ��͵�ƽ����
		while (dht11_data == 0);
		
		// �ȴ��ߵ�ƽ����
		while (dht11_data == 1);
		
		// ���ն�ȡ�������
		RHH   = DHT11_RBYTE();
		RHL   = DHT11_RBYTE();
		CH    = DHT11_RBYTE();
		CL    = DHT11_RBYTE();
		CHECK = DHT11_RBYTE();
		
		// ���ս��������͵�ƽ
		dht11_low();
		
		tim6_delay_us(50);
		
		// ���ߵ�ƽ �������ߴ��ڿ���״̬
		dht11_high();
		
		// ��������׼ȷ��
		if (RHH + RHL + CH + CL == CHECK)
		{
			RH_H = RHH;
			RH_L = RHL;
			C_H  = CH;
			C_L  = CL;
		}
	}
	data[3] = RH_H;
	data[2] = RH_L;
	data[1] = C_H;
	data[0] = C_L;
	
}

void READ_DATA(float * temp,float * humi)
{
	GET_DATA();
	
}
