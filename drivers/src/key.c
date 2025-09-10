#include "key.h"
#include "gizwits_product.h"
#include "tim.h"

void WiFi_Key_Init(void) //������ʼ��
{ 

 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);// 


	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 

	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void WiFi_Key_Switch(void)
{	//�ڶ�Ӧͷ�ļ�����Ӻ� #define WiFi_Key   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����
	if(WiFi_Key == RESET)
	{
		 tim6_delay_ms(10);
		if(WiFi_Key == RESET)
		{										
			gizwitsSetMode(WIFI_SOFTAP_MODE);  //WIFI_AIRLINK_MODE  WIFI_SOFTAP_MODE ����ģʽѡ��
			printf("WIFI_SOFTAP_MODE OK \r\n");
		}
		while(!WiFi_Key);
	}

}
