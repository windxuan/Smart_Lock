#ifndef __BITBAND_H_
#define __BITBAND_H_

#include "stm32f10x.h"

/*

�����ӳ�乫ʽ�����˱������е�ÿ��������ζ�Ӧλ��������Ӧλ�ģ�

bit_word_addr = bit_band_base + (byte_offset * 32) + (bit_number * 4) 

���У�
bit_word_addr �Ǳ����洢�������ֵĵ�ַ����ӳ�䵽ĳ��Ŀ��λ��
bit_band_base �Ǳ���������ʼ��ַ��
byte_offset   �ǰ���Ŀ��λ���ֽ���λ��������
bit_number    ��Ŀ��λ����λ��(0-31)

*/

// ������־��棺extended constant initialiser used
#if defined(__CC_ARM)
#pragma diag_suppress 1296
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#endif

// ����λ�α������ֵ�ַ
#define BIT_WORD_ADDR(bit_band_base, byte_offset, bit_number) \
	((bit_band_base) + ((byte_offset) * 32) + ((bit_number) * 4))

// ����&GPIOC->ODR[9]����ַΪ 0x4001100C
// �ɼĴ�����ַ������λ�α�������ʼ��ַ��
// (0x4001100C & 0xF0000000) + 0x02000000 = 0x42000000
// �ɼĴ�����ַ�������ֽ�ƫ�Ƶ�ַ��
// 0x4001100C & 0x000FFFFF = 0x0001100C

// ��λ������ַ�е�ĳ��λ������������Ӧ���������ֵ�ַ
#define BIT_TO_ALIAS(addr, bit_number) \
	BIT_WORD_ADDR((((uint32_t)(addr)) & 0xF0000000) + 0x02000000, \
	((uint32_t)(addr)) & 0x000FFFFF, bit_number)

// �� GPIO �˿� IDR �� ODR λ����
// ָ��
#define PPAin(bit_number)  ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOA->IDR, bit_number))
#define PPBin(bit_number)  ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOB->IDR, bit_number))
#define PPCin(bit_number)  ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOC->IDR, bit_number))
#define PPDin(bit_number)  ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOD->IDR, bit_number))

#define PPAout(bit_number) ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOA->ODR, bit_number))
#define PPBout(bit_number) ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOB->ODR, bit_number))
#define PPCout(bit_number) ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOC->ODR, bit_number))
#define PPDout(bit_number) ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOD->ODR, bit_number))

// λ
#define PAin(bit_number)   (*PPAin(bit_number))
#define PBin(bit_number)   (*PPBin(bit_number))
#define PCin(bit_number)   (*PPCin(bit_number))
#define PDin(bit_number)   (*PPDin(bit_number))

#define PAout(bit_number)  (*PPAout(bit_number))
#define PBout(bit_number)  (*PPBout(bit_number))
#define PCout(bit_number)  (*PPCout(bit_number))
#define PDout(bit_number)  (*PPDout(bit_number))

#endif  // __BITBAND_H_
