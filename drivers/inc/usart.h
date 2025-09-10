#ifndef __USART_H_
#define __USART_H_

#include "stm32f10x.h"

#define USART1_RECV_BUF_SIZE 128
extern volatile uint8_t usart1_recv_buf[USART1_RECV_BUF_SIZE];
extern volatile int usart1_recv_index;
extern volatile int usart1_get_index;
extern volatile int usart1_idle_flag;


void USART2_Config(void);
int usart1_putchar(int data);
int usart1_getchar(void);
void USARTx_SendNByte(USART_TypeDef* USARTx ,uint8_t *data, uint8_t len);
void usart1_init(uint32_t baudrate);

#if 0   // 喇叭

#define USART3_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void USART3_SendString(u8 *DAT,u8 len);
void USART3_SendData(u8 data);
void USART3_Init(u32 bound);

#endif
void USART3_Init(u32 bound);


// USART3 指纹
#define USART3_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		400					//最大发送缓存字节数
#define USART3_RX_EN 			1								//0,不接收;1,接收.

extern volatile u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern volatile u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern volatile u16 USART3_RX_STA;   				 //接收数据状态

void USART3_Config(uint32_t boudrate);
void usart3_send(uint8_t data);

#endif
