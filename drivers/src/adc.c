#include "adc.h"

// 查看原理图得：
// ADKey - PC1 - ADC123_IN11

/**
 * 简述：初始化 ADC1 通道11
 * 参数：无
 * 返回值：无
 **/
 
void adc1_ch11_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	
	// 使能 GPIOC 和 ADC1 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);
	
	// 配置 ADC 预分频
	// void RCC_ADCCLKConfig(uint32_t RCC_PCLK2);
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
	
	// 配置 PC1 为模拟输入
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	// 配置 ADC1
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;  // 单次转换
	ADC_InitStruct.ADC_DataAlign          = ADC_DataAlign_Right;  // 数据右对齐
	ADC_InitStruct.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;  // 不通过外部触发转换
	ADC_InitStruct.ADC_Mode               = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel       = 1;
	ADC_InitStruct.ADC_ScanConvMode       = DISABLE;  // 禁用扫描模式
	// void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
	ADC_Init(ADC1, &ADC_InitStruct);
	
	// 使能 ADC1
	// void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);
	ADC_Cmd(ADC1, ENABLE);
	
	// 复位校准
	// void ADC_ResetCalibration(ADC_TypeDef* ADCx);
	// FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx);
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	
	// 校准
	// void ADC_StartCalibration(ADC_TypeDef* ADCx);
	// FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/**
 * 简述：启动并获取 ADC1 通道11 的转换结果
 * 参数：无
 * 返回值：转换结果
 **/
uint16_t adc1_ch11_get_adkey_value(void)
{
	// 配置规则通道
	// void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5);
	
	// 软件启动转换
	// void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
	// FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetSoftwareStartConvStatus(ADC1) == SET);  // 等待开始转换
	
	// 等待转换结束
	// FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	
	// 获取转换结果。EOC 标志会在读取 DR 时自动清除，因此无需手动清除
	// uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);
	return ADC_GetConversionValue(ADC1);
}
