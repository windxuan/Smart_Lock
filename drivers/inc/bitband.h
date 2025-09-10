#ifndef __BITBAND_H_
#define __BITBAND_H_

#include "stm32f10x.h"

/*

下面的映射公式给出了别名区中的每个字是如何对应位带区的相应位的：

bit_word_addr = bit_band_base + (byte_offset * 32) + (bit_number * 4) 

其中：
bit_word_addr 是别名存储器区中字的地址，它映射到某个目标位。
bit_band_base 是别名区的起始地址。
byte_offset   是包含目标位的字节在位段里的序号
bit_number    是目标位所在位置(0-31)

*/

// 避免出现警告：extended constant initialiser used
#if defined(__CC_ARM)
#pragma diag_suppress 1296
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#endif

// 计算位段别名区字地址
#define BIT_WORD_ADDR(bit_band_base, byte_offset, bit_number) \
	((bit_band_base) + ((byte_offset) * 32) + ((bit_number) * 4))

// 例：&GPIOC->ODR[9]，地址为 0x4001100C
// 由寄存器地址计算其位段别名区起始地址：
// (0x4001100C & 0xF0000000) + 0x02000000 = 0x42000000
// 由寄存器地址计算其字节偏移地址：
// 0x4001100C & 0x000FFFFF = 0x0001100C

// 由位段区地址中的某个位，计算其所对应别名区的字地址
#define BIT_TO_ALIAS(addr, bit_number) \
	BIT_WORD_ADDR((((uint32_t)(addr)) & 0xF0000000) + 0x02000000, \
	((uint32_t)(addr)) & 0x000FFFFF, bit_number)

// 各 GPIO 端口 IDR 和 ODR 位定义
// 指针
#define PPAin(bit_number)  ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOA->IDR, bit_number))
#define PPBin(bit_number)  ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOB->IDR, bit_number))
#define PPCin(bit_number)  ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOC->IDR, bit_number))
#define PPDin(bit_number)  ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOD->IDR, bit_number))

#define PPAout(bit_number) ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOA->ODR, bit_number))
#define PPBout(bit_number) ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOB->ODR, bit_number))
#define PPCout(bit_number) ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOC->ODR, bit_number))
#define PPDout(bit_number) ((volatile uint32_t *) BIT_TO_ALIAS(&GPIOD->ODR, bit_number))

// 位
#define PAin(bit_number)   (*PPAin(bit_number))
#define PBin(bit_number)   (*PPBin(bit_number))
#define PCin(bit_number)   (*PPCin(bit_number))
#define PDin(bit_number)   (*PPDin(bit_number))

#define PAout(bit_number)  (*PPAout(bit_number))
#define PBout(bit_number)  (*PPBout(bit_number))
#define PCout(bit_number)  (*PPCout(bit_number))
#define PDout(bit_number)  (*PPDout(bit_number))

#endif  // __BITBAND_H_
