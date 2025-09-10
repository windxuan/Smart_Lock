#include "dht11.h"

// 查看原理图得：
// DHT11 与 PA8 连接

// 数据存放
volatile unsigned int data[4];

/**
 * 简述 ： DHT11 输入配置
 * 参数 ： 无
 * 返回值 ： 无
 **/
 
void dhtll_init_in(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// 打开 GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE );
	
	// 配置 PA8
	//void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

/**
 * 简述 ： DHT11 输出配置
 * 参数 ： 无
 * 返回值 ： 无
 **/
 
void dhtll_init_out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// 打开 GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE );
	
	// 配置 PA8
	//void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

/**
 * 简述 ：主机发送开始信号
 * 参数 ： 无
 * 返回值 ： 无
 **/
void DHT11_start(void)
{
	// 输出模式
	dhtll_init_out();
	
	// 先拉高电平
	dht11_high();
	tim6_delay_ms(30);
	
	// 拉低电平
	dht11_low();
	tim6_delay_ms(18);
	
	// 拉高电平
	dht11_high();
	tim6_delay_us(30);
	
	// 输入模式
	dhtll_init_in();
}

/**
 * 简述 ： 获取一个字节
 * 参数 ： 无
 * 返回值 ： 获取到的字节
 **/
char DHT11_RBYTE(void)
{
	unsigned int i = 0;
	unsigned int data;
	
	for (i = 0; i < 8; i++)
	{
		// 等待信号变成高电平
		while (dht11_data == 0);
		
		// 延时30us
		tim6_delay_us(30);
		
		// data左移一位
		data <<= 1;
		
		if(dht11_data == 1)
		{
			data |= 1;
		}
		
		// 等待低电平到来
		while (dht11_data == 1);
	}
	return data;
}

/**
 * 简述 ： 获取温湿度的所有信息
 * 参数 ： 无
 * 返回值 ： 无
 **/
void GET_DATA(void)
{
	unsigned char RH_H, RH_L, C_H, C_L, CHECK;
	unsigned int RHH, RHL, CH, CL;
	
	// 主机发送开始信号
	DHT11_start();
	
	// 判断DH11T的回应信号, 0 - 响应  1 - 未响应
	if (dht11_data == 0)
	{
		// 等待低电平结束
		while (dht11_data == 0);
		
		// 等待高电平结束
		while (dht11_data == 1);
		
		// 接收读取五个数据
		RHH   = DHT11_RBYTE();
		RHL   = DHT11_RBYTE();
		CH    = DHT11_RBYTE();
		CL    = DHT11_RBYTE();
		CHECK = DHT11_RBYTE();
		
		// 接收结束后拉低电平
		dht11_low();
		
		tim6_delay_us(50);
		
		// 拉高电平 让数据线处于空闲状态
		dht11_high();
		
		// 检验数据准确性
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
