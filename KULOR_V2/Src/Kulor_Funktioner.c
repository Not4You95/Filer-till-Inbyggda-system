#include "stdint.h"
#include "gpio.h"
#include "stm32f3xx_hal.h"
#include "stdbool.h"
#include "usart.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stm32f3_discovery.h"
#include "Kulor_Funktioner.h"
#include "crc.h"
#include  "main.h"

RTC_HandleTypeDef RtcHandle;
ITStatus UartReady = RESET;
TIM_HandleTypeDef htim1;

//typedef enum { minits,houer,day,year,mouth} sates_clk;
//sates_clk State, NextState;
  char *SetDate[3];
  char *SetTime[2];
  char NewLine[]="\n\r";
  char tempInputDate[10];
  char tempInputTime[6];
  
  uint8_t                PreambleCount=0;
  uint8_t               Temprature[10];
  uint8_t               humidity[7];
  uint32_t               RecivedPacket[32];
  uint8_t                BitCount = 0;
  uint32_t              uwIC2Value1=0;
 
  uint8_t               CrcCode=10;
  bool                  PreambleFlag=false;
  uint32_t               PulsOneLength=0;
  
//////////////////////////////////////////////////
  uint32_t              data_array[40];


///////////////////////////////////////////////////

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){
  UartReady = SET; // Set the transmiton flag to complete 
   //printf("Hello Tx\n");
}
/////
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){
  UartReady = SET; // Set the transmiton flag to complete 
  //printf("Hello Rx\n");
  
}
////Send text to uset throw uart/////
void SendToSerial(uint8_t *text, uint8_t size){  
 
   
 
  
  if(HAL_UART_Transmit_IT(&huart3, (uint8_t*)text, size) != HAL_OK){
      Error_Handler();
      
    }
    while(UartReady != SET)
    {
    }
    UartReady = RESET;

 }
///Recive info from user throw uart///////
void ReciveFromUser_clk(char *temp,uint8_t size){

  
  printf("Recive Hello  %d  %d!!!\n",sizeof(temp),size);
  
  if(HAL_UART_Receive_IT(&huart3, (uint8_t*)temp, size) != HAL_OK){
      Error_Handler();      
    }
    while(UartReady != SET)
    {
    }
    UartReady = RESET;
     
    
  
  printf("Input: %s %d\n",temp,sizeof(temp));
     
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void assert_failed(uint8_t *file, uint32_t line){
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RTC_CalendarConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
  
  
  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2014 */
 static uint8_t year;
 static uint8_t date,minits,hour,mouth;
  year = (atoi(SetDate[0])-1994);
  
   mouth = atoi(SetDate[1]); 
  
  if(atoi(SetDate[2])>15){
    date = (atoi(SetDate[2])+11);
  }
  else{
    date = atoi(SetDate[2]);
  }
    
  if(atoi(SetTime[1])>15){
    minits = (atoi(SetTime[1])+30);
  }
  else{
    minits = atoi(SetTime[1]);
  }
  
  if(atoi(SetTime[0])>15){
    hour = (atoi(SetTime[0])+6);
  }
  else{
    hour = atoi(SetTime[0]);    
  }
  //printf("\n\nHour: %d:%d  Date: %d-%d-%d\n",hour,minits,date,mouth,year);
  
  
  sdatestructure.Year = year;//atoi(SetDate[0]);//0x14;
  sdatestructure.Month = mouth;
  sdatestructure.Date = date;//atoi(SetDate[2]);//0x18;
  sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;
  
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    // Initialization Error 
    Error_Handler();
  }

  /*##-2- Configure the Time #################################################*/
  /* Set Time: 02:00:00 */
  stimestructure.Hours = hour;//atoi(SetTime[0]);//0x02;
  stimestructure.Minutes = minits;//atoi(SetTime[1]);
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT_24;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    // Initialization Error 
    Error_Handler();
  }

  /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
  uint8_t Text[] = "The clock in now set!\n\r";
  SendToSerial(Text,sizeof(Text));
  
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
  printf("Date: %2d-%2d-%2d\n\n",  sdatestructureget.Date,sdatestructureget.Month, 2000 + sdatestructureget.Year);
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RTC_CLOCK_SETINGS()
{
 
  BSP_LED_Init(LED6);// green
  BSP_LED_Init(LED4);// Blue
  BSP_LED_Init(LED3);// Red
  BSP_LED_Init(LED5);// Orage 
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
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
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
  BSP_LED_On(LED6);
 

  //##-2- Check if Data stored in BackUp register1: No Need to reconfigure RTC#
  // Read the Back Up Register 1 Data 
  if (HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) != 0x32F2)
  {
    // Configure RTC Calendar 
   //
  }
  else
  {
    // Check if the Power On Reset flag is set 
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
    {
      // Turn on LED4: Power on reset occurred 
      BSP_LED_On(LED4);
    }
    // Check if Pin Reset flag is set 
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
    {
      // Turn on LED5: External reset occurred 
      BSP_LED_On(LED5);
    }
    // Clear source Reset Flag 
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }
  
}


 void SetDisplayNumber(){
    
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetDisplayClock(uint8_t number[])
{
  
  if(number[0]==1){
    HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_SET);
           
            
  }
  else{
    HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
  }
   if(number[1]==1){
           
            HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_SET);
           
  }
   else{
     HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
   }
   if(number[2]==1){
            
            HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_SET);           
    
  }
   else{
          HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
   }
  if(number[3]==1){
           
            HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_SET);
             
  }
  else{
     HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
  }

}

void ShowNumberOnDispaly(uint8_t number[]){
  RTC_TimeTypeDef stimestructureget;
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  uint8_t houer = stimestructureget.Hours;
  uint8_t minits = stimestructureget.Minutes;
  printf("Time: %d:%d\n",houer,minits);
  
     HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);
         
  
}

void SetDisplayTemp(uint8_t number[])
{
  HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_SET);
  if(number[0]==1){
    HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_SET);
           
            
  }
  else{
    HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
  }
   if(number[1]==1){
           
            HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_SET);
           
  }
   else{
     HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
   }
   if(number[2]==1){
            
            HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_SET);           
    
  }
   else{
          HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
   }
  if(number[3]==1){
           
            HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_SET);
             
  }
  else{
     HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
  }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void set_clock_serial(void){
 
   char *token;
   
   int DateCount=0,TimeCount=0;
   
  for(int i=0;i<2;i++){
   
    switch(i){
      
    case 0:// set the time RTC throw uart
      
      uint8_t text[]= "\n Input Date yyyy-mm-dd:\n\r";
      printf("I:%d\n",i);
      SendToSerial(text,sizeof(text));
      ReciveFromUser_clk(tempInputDate,10);
      SendToSerial(tempInputDate,sizeof(tempInputDate));
      SendToSerial(NewLine,sizeof(NewLine));
   /* get the first token */
      token = strtok(tempInputDate, "-");
     // printf("%s\n",token);
      
     while( token != NULL ) 
   {     
      SetDate[DateCount] = token;      
      DateCount++;    
      token = strtok(NULL, "-");    
      
   }
   printf("test: %s",SetDate);
     
      break;
      
      
    case 1:// set the time RTC throw uart
       uint8_t text_h[]= "\n Input Time hh:mm:\n\r";
       printf("I:%d\n",i);
      SendToSerial(text_h,sizeof(text_h));      
      ReciveFromUser_clk(tempInputTime,5);
      SendToSerial(tempInputTime,sizeof(tempInputTime));
      SendToSerial(NewLine,sizeof(NewLine));
      token = strtok(tempInputTime, ":");
       while( token != NULL ) 
   {     
      SetTime[TimeCount] = token; 
      //printf("%s\n",SetTime[TimeCount]);
      TimeCount++;    
      token = strtok(NULL, ":");  
       
      
   }
    
      break;  
   
    default: break;
      
    }
    
 }

 int test2 = atoi(SetDate[0]); 
 printf("Test: %d %s\n\n\n",test2,SetDate[0]);
 
 RTC_CalendarConfig(); 


  
}
/////////////TIM1 INTERUPTS F�R PULS BRED///////////////////////////////////////
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
  
// HAL_TIM_IC_Stop_IT(&htim1,TIM_CHANNEL_2);
  if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_2){   
     BitCount++;

      uwIC2Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
      //CalculateTempraturePacket(uwIC2Value1);
      
       printf("\nIC INPUT: %d\n",uwIC2Value1);
       
      // printf("Array: %d\nBitCount: %d\n\n\n",RecivedPacket[BitCount],BitCount);
       printf("BitCount: %d\nPreamble: %d\n",BitCount,PreambleCount);
       
       if(9000<uwIC2Value1<9400 && BitCount<10){
         PreambleCount++;
        // PulsOneLength+=uwIC2Value1+PulsOneLength;
       }
       if(BitCount==10 && PreambleCount==9){
         PreambleFlag=true;
       }
       
       if(PreambleFlag){
         RecivedPacket[BitCount-10] = uwIC2Value1;
         printf("Array: %d\nBitCount: %d\n\n\n",RecivedPacket[BitCount-10],(BitCount-10));

        
       }
       if(BitCount == 50){
         CalculateTempraturePacket(RecivedPacket);
         BitCount=0;
         PreambleCount=0;
         PreambleFlag=false;
       }
     
    }
    if(PreambleCount<9 && BitCount>9){
             BitCount=0;
           PreambleCount=0;
           PreambleFlag=false;
    }
 
     HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_2);
    }

int CalculatePulsWithd(uint32_t puls){
  
  
      
      if(14900< puls && puls <15200 || puls==0)
      {
         return 0;
      }
      else if(6000<puls && puls <6100 || puls==1){
        return 1;
      }
      
      return 0;
      
}

void CalculateTempraturePacket(uint32_t PulsInput[]){
  uint32_t temp[40];
    data_array[0] = 0;
  data_array[1] = 1;
  data_array[2] = 0;
  data_array[3] = 0;
  data_array[4] = 1;
  data_array[5] = 0;
  data_array[6] = 0;
  data_array[7] = 0;
    
  data_array[8] = 0;
  data_array[9] = 1;
  data_array[10] = 1;
  data_array[11] = 1;
  data_array[12] = 0;
  data_array[13] = 0;
  data_array[14] = 0;
  data_array[15] = 0;
    
  data_array[16] = 1;
  data_array[17] = 1;
  data_array[18] = 0;
  data_array[19] = 1;
  data_array[20] = 0;
  data_array[21] = 1;
  data_array[22] = 1;
  data_array[23] = 1;
    
  data_array[24] = 0;
  data_array[25] = 0;
  data_array[26] = 1;
  data_array[27] = 0;
  data_array[28] = 0;
  data_array[29] = 0;
  data_array[30] = 1;
  data_array[31] = 0;
    
  data_array[32] = 0;
  data_array[33] = 1;
  data_array[34] = 0;
  data_array[35] = 1;
  data_array[36] = 1;
  data_array[37] = 1;
  data_array[38] = 0;
  data_array[39] = 1;
  
  for(int i=0;i<40;i++){
   temp[i]=CalculatePulsWithd(PulsInput[i]);
    printf("%d\n", PulsInput[i]);
    
  }
  printf("Temprature: \n");
  for(int i=13;i<23;i++){
    Temprature[i-13]= temp[i];
    printf("%d\n",Temprature[i-13]);
    
  }
  
  printf("\nLuft: ");
  for(int a=23;a<30;a++){
    humidity[a-24]= temp[a];
    printf("%d",humidity[a-24]);
  }
  
  printf("\nCRC: ");
  for(int i=33;i<40;i++){   
    printf("%d",temp[i]);
  }
  
  for(int i=0;i<40;i++){
    if(temp[i]==data_array[i])
    {
    }
    else{
      printf("\nBit fel nr: %d\n Temp: %d Data: %d\n\n",i,temp[i],data_array[i]);
    }
    
  }
  
  CrcCode = HAL_CRC_Calculate(&hcrc, temp, 40);
  
  printf("\nCRC: %d\n",CrcCode);

   
}

