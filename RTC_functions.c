
#include "stdint.h"
#include "stm32f3xx_hal.h"
#include "stdbool.h"
#include "main.h"
#include "stdlib.h"
#include "stdio.h"
#include "RTC_functions.h"


RTC_HandleTypeDef RtcHandle;

/*uint8_t aShowTime[50] = {0};
uint8_t aShowDate[50] = {0};*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RTC_CalendarConfig(char time,char day)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
  
  
  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2014 */
  sdatestructure.Year = day;;
  sdatestructure.Month = RTC_MONTH_FEBRUARY;
  sdatestructure.Date = time;
  sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;
  printf("Day: %c\n",day);
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    // Initialization Error 
    Error_Handler();
  }

  /*##-2- Configure the Time #################################################*/
  /* Set Time: 02:00:00 */
  /*stimestructure.Hours = hour;
  stimestructure.Minutes = minuter;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT_24;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;*/

  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    // Initialization Error 
    Error_Handler();
  }

  /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate)
{
  
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  printf("Time: %2d:%2d:%2d\n", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  printf("Date: %2d-%2d-%2d\n\n", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
  
  printf((char *)showtime, "%2d:%2d:%2d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  
  printf((char *)showdate, "%2d-%2d-%2d\n", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RTC_CLOCK_SETINGS()
{
   ////////////////////////////////////////////////////
  /*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
      - Hour Format    = Format 24
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */ 
  RtcHandle.Instance = RTC; 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
 // RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
 // RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
///////////////////////////////////////////////////////
   

  if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    // Initialization Error 
    Error_Handler();
  }
  
  //Turn on LED6
 // BSP_LED_On(LED6);
  
 // RTC_CalendarConfig();

  //##-2- Check if Data stored in BackUp register1: No Need to reconfigure RTC#
  // Read the Back Up Register 1 Data 
  if (HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) != 0x32F2)
  {
    // Configure RTC Calendar 
   // RTC_CalendarConfig();
  }
  else
  {
    // Check if the Power On Reset flag is set 
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
    {
      // Turn on LED4: Power on reset occurred 
   //   BSP_LED_On(LED4);
    }
    // Check if Pin Reset flag is set 
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
    {
      // Turn on LED5: External reset occurred 
      //BSP_LED_On(LED5);
    }
    // Clear source Reset Flag 
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }
  
}