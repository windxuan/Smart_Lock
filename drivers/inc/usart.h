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

#if 0   // ����

#define USART3_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART3_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void USART3_SendString(u8 *DAT,u8 len);
void USART3_SendData(u8 data);
void USART3_Init(u32 bound);

#endif
void USART3_Init(u32 bound);


// USART3 ָ��
#define USART3_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART3_RX_EN 			1								//0,������;1,����.

extern volatile u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern volatile u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern volatile u16 USART3_RX_STA;   				 //��������״̬

void USART3_Config(uint32_t boudrate);
void usart3_send(uint8_t data);

#endif
