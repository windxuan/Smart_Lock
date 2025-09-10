/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "led.h"
#include "tim.h"
#include "dht11.h"
#include "oled.h"
#include "syn6288.h"
#include "as608.h"

static uint32_t timerMsCount;
static uint8_t temp;

/** Current datapoint */
dataPoint_t currentDataPoint;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
    uint8_t i = 0;
    dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
    moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
    protocolTime_t *ptime = (protocolTime_t *)gizdata;
    
#if MODULE_TYPE
    gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
    moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

    if((NULL == info) || (NULL == gizdata))
    {
        return -1;
    }

    for(i=0; i<info->num; i++)
    {
        switch(info->event[i])
        {
        case EVENT_LEDonoff:
            currentDataPoint.valueLEDonoff = dataPointPtr->valueLEDonoff;
            GIZWITS_LOG("Evt: EVENT_LEDonoff %d \n", currentDataPoint.valueLEDonoff);
            if(0x01 == currentDataPoint.valueLEDonoff)
            {
            //user handle
							LED_ON();
							fig1();
							tim6_delay_ms(1000);
							total();
							//Add_FR();
						//	printf("LEDR OPEN\r\n");
            }
            else
            {
            //user handle  
							LED_OFF();
							fig2();
//							temp = press_FR();
//							if(temp == 1)
//							{
//								turn_pwm_1();
//								total();
//							}
							//printf("LEDR CLOSE\r\n");
            }
            break;
        case EVENT_DOOR:
            currentDataPoint.valueDOOR = dataPointPtr->valueDOOR;
            GIZWITS_LOG("Evt: EVENT_DOOR %d \n", currentDataPoint.valueDOOR);
            if(0x01 == currentDataPoint.valueDOOR)
            {
            //user handle
							turn_pwm_1();
							open();
							tim6_delay_ms(1000);
							total();
						  //speak();
							printf("DOOR OPEN\r\n");
            }
            else
            {
            //user handle   
							turn_pwm_3();
							close();
							tim6_delay_ms(1000);
							total();
							printf("DOOR CLOSE\r\n");								
            }
            break;




        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:
 
            break;
        case WIFI_DISCON_ROUTER:
 
            break;
        case WIFI_CON_M2M:
 
            break;
        case WIFI_DISCON_M2M:
            break;
        case WIFI_RSSI:
            GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
            break;
        case TRANSPARENT_DATA:
            GIZWITS_LOG("TRANSPARENT_DATA \n");
            //user handle , Fetch data from [data] , size is [len]
            break;
        case WIFI_NTP:
            GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
            break;
        case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
#if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",gprsInfoData->Type);
#else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
#endif
        break;
        default:
            break;
        }
    }

    return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
		volatile float RH_H,RH_L,TC_H,TC_L,R,T;
	  GET_DATA();
		RH_H = data[3];
		RH_L = data[2] / 01100100;
		TC_H = data[1];
		TC_L = data[0] / 01100100;
		R = RH_H + RH_L;
		T = TC_H + TC_L;
    currentDataPoint.valueTEM =T;//Add Sensor Data Collection
		//printf("%f\r\n",T);
    currentDataPoint.valueWEM = R;//Add Sensor Data Collection
		//printf("%f\r\n",R);

    
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
    currentDataPoint.valueLEDonoff = ;
    currentDataPoint.valueDOOR = ;
    currentDataPoint.valueTEM = ;
    currentDataPoint.valueWEM = ;
    */

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{

	__set_FAULTMASK(1);	//关闭所有中断
	  NVIC_SystemReset();	//复位函数
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
//void TIMER_IRQ_FUN(void)
//{
//  gizTimerMs();
//}
void TIM3_IRQHandler(void)
{
	if( TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		gizTimerMs();
	}

}
/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
//void UART_IRQ_FUN(void)
//{
//  uint8_t value = 0;
//  //value = USART_ReceiveData(USART2);//STM32 test demo
//  gizPutData(&value, 1);
//}
void USART2_IRQHandler(void)
{
  uint8_t value = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		value = USART_ReceiveData(USART2);//STM32 test demo
		gizPutData(&value, 1);
	}

}

/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
   uint32_t i = 0;
	

    if(NULL == buf)
    {
        return -1;
    }
    
    for(i=0; i<len; i++)
    {
        USART_SendData(USART2, buf[i]);
    		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
					;
        if(i >=2 && buf[i] == 0xFF)
        {
    					USART_SendData(USART2,0x55);
    					while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        }
    }


#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);

        if(i >=2 && buf[i] == 0xFF)
        {
            GIZWITS_LOG("%02x ", 0x55);
        }
    }
    GIZWITS_LOG("\n");

#endif
		

		return len;
}


