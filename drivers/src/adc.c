#include "adc.h"

// �鿴ԭ��ͼ�ã�
// ADKey - PC1 - ADC123_IN11

/**
 * ��������ʼ�� ADC1 ͨ��11
 * ��������
 * ����ֵ����
 **/
 
void adc1_ch11_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	
	// ʹ�� GPIOC �� ADC1 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);
	
	// ���� ADC Ԥ��Ƶ
	// void RCC_ADCCLKConfig(uint32_t RCC_PCLK2);
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
	
	// ���� PC1 Ϊģ������
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	// ���� ADC1
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;  // ����ת��
	ADC_InitStruct.ADC_DataAlign          = ADC_DataAlign_Right;  // �����Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;  // ��ͨ���ⲿ����ת��
	ADC_InitStruct.ADC_Mode               = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel       = 1;
	ADC_InitStruct.ADC_ScanConvMode       = DISABLE;  // ����ɨ��ģʽ
	// void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
	ADC_Init(ADC1, &ADC_InitStruct);
	
	// ʹ�� ADC1
	// void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);
	ADC_Cmd(ADC1, ENABLE);
	
	// ��λУ׼
	// void ADC_ResetCalibration(ADC_TypeDef* ADCx);
	// FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx);
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	
	// У׼
	// void ADC_StartCalibration(ADC_TypeDef* ADCx);
	// FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/**
 * ��������������ȡ ADC1 ͨ��11 ��ת�����
 * ��������
 * ����ֵ��ת�����
 **/
uint16_t adc1_ch11_get_adkey_value(void)
{
	// ���ù���ͨ��
	// void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5);
	
	// �������ת��
	// void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
	// FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetSoftwareStartConvStatus(ADC1) == SET);  // �ȴ���ʼת��
	
	// �ȴ�ת������
	// FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	
	// ��ȡת�������EOC ��־���ڶ�ȡ DR ʱ�Զ��������������ֶ����
	// uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);
	return ADC_GetConversionValue(ADC1);
}
