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
#include "math.h"

RTC_HandleTypeDef RtcHandle;
ITStatus UartReady = RESET;
TIM_HandleTypeDef htim1;
RTC_DateTypeDef sdatestructure;
RTC_TimeTypeDef stimestructure;
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
uint32_t               RecivedPacket[350];
uint16_t                BitCount = 0;
uint16_t                arrayCount=0;
uint32_t              uwIC2Value1=0;
uint16_t                shortPeriod=600,LongPeriod=1500,space=20;
uint8_t                TempratureValue[4];
uint16_t               BitValue[]= {512,256,128,64,32,16,8,4,2,1};
uint8_t                HumidityValue[3];
uint8_t               CrcCode=10;
uint32_t               PulsOneLength=0;
uint32_t              data_array[40];


///////////////////////////////////////////////////

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){
  UartReady = SET; // Set the transmiton flag to complete 
  
}
/////
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){
  UartReady = SET; // Set the transmiton flag to complete 
  
  
}


////Send text to uset throw uart/////
void SendToSerial(uint8_t *text, uint8_t size){  
  /* Denna fuktion sänder man  in en array med värden som man vill sända t*/
 
  HAL_UART_Transmit(&huart3, (uint8_t*)text, size,5000);
    
  
}
///Recive info from user throw uart///////
void ReciveFromUser_clk(char *temp,uint8_t size){
   
  
  if(HAL_UART_Receive_IT(&huart3, (uint8_t*)temp, size) != HAL_OK){
    Error_Handler();      
  }
  while(UartReady != SET)
  {
  }
  UartReady = RESET;
  
  
  
  
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RTC_CalendarConfig(void)
{
  
  // Sätter RTC klockan 
  sdatestructure.Year = (atoi(SetDate[0])-2000);//atoi(SetDate[0]);//0x14;
  sdatestructure.Month = atoi(SetDate[1]);
  sdatestructure.Date = atoi(SetDate[2]);//atoi(SetDate[2]);//0x18;
  sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;
  
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BIN) != HAL_OK)
  {
    // Initialization Error 
    Error_Handler();
  }
  
  
  stimestructure.Hours = atoi(SetTime[0]);//atoi(SetTime[0]);//0x02;
  stimestructure.Minutes = atoi(SetTime[1]);//atoi(SetTime[1]);
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT_24;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BIN) != HAL_OK)
  {
    // Initialization Error 
    Error_Handler();
  }
  
  /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
  
  if(stimestructure.Hours == atoi(SetTime[0]) && stimestructure.Minutes == atoi(SetTime[1])){
    uint8_t text_ok[] ="The clock is now set!!\n\r";
    SendToSerial(text_ok,sizeof(text_ok));
    
  }
  else{
    uint8_t text_fail[] = "Can not set the clock!!\n\r";
    SendToSerial(text_fail,sizeof(text_fail)); 
  }
  HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_2);
  
}
void ShowNumberOnDispaly(uint8_t number)
{
  
  
  //vilken sifra som ska visas
  
  switch(number){
    
  case 0: HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);          
  HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET); 
  break;
  
  case 1: HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);          
  HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
  break;
  case 2:
    HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);          
    HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
    break;
  case 3:
    HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);          
    HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
    break;
  case 4:
    HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);          
    HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
    break;
  case 5:
    HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);          
    HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
    break;
  case 6:
    HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);          
    HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
    break;
  case 7:
    HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);          
    HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
    break;
  case 8: 
    HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);          
    HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
    break;
  case 9:
    HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);          
    HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
    break;
    
  case 10:  HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);          
  HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
  break;
  
  case 11:  HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);          
  HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
  break;       
  
  default: break;
  }
  
  
}

void UppDateDisplay(uint8_t number)
{
  static uint32_t last_second = 0;
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
  
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
  
  
  uint8_t houre1=1,houre2=2,minits1=3,minits2=4;  
  
  // Använder GetTick för att få reda på när en sekud har gåt
  uint32_t current_second = HAL_GetTick();
  if ((current_second - last_second)/2 > 500)
  {    
    last_second = current_second;        
    HAL_GPIO_TogglePin(GPIOC,Kolon_Pin);
    
  }
  
  // Vilken 7-seg man vill tända 
  if(number == 0){
    HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_SET);
    
   
    ShowNumberOnDispaly((stimestructureget.Hours/10 ));
    
    
  }
  else{
    HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
  }
  
  if(number == 1){
    
    HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_SET);    
    
    ShowNumberOnDispaly((stimestructureget.Hours%10));
    
  }
  else{
    HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
  }
  
  if(number == 2){
    
    HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_SET); 
    // printf("Test: %d\n",(stimestructureget.Minutes/10));
    ShowNumberOnDispaly((stimestructureget.Minutes/10));
    
  }
  else{
    HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
  }
  if(number == 3 ){
    
    HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_SET);
    
    ShowNumberOnDispaly((stimestructureget.Minutes%10));
    
  }
  else{
    HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
  }
  
  if(number == 4){
    HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_SET);
    if(TempratureValue[5]==1){
      ShowNumberOnDispaly(10);
    }
    else{
      ShowNumberOnDispaly(11);
    }
    
  }
  else{
    HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
    
  }
  
  if(number ==5 ){
    HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_SET);
    ShowNumberOnDispaly(TempratureValue[0]);
    
  }
  else{
    
    HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
  }
  
  if(number == 6){
    HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_SET);
    ShowNumberOnDispaly(TempratureValue[1]);
    HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_RESET);
    
  }
  else {
    HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
  }
  
  if(number == 7){
    HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_SET);
    ShowNumberOnDispaly(TempratureValue[2]);
  }
  else{
    HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
  }
  
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RTC_CalendarShow()
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////

void getTempratue(){
  uint8_t temp=0,value1,value2,valueTemp,decimal;
  for(int i=0;i<sizeof(Temprature);i++){
    
    if(Temprature[i]==1){// Kollar om det finns en 1 i så fall aderas bit positionens värde
      temp += BitValue[i];     
      
    }   
    
  }
  // Sparar värdena 
  decimal = temp%10;
  valueTemp = temp/10;
  value2 = valueTemp%10;
  value1 = valueTemp/10;
  
  
  TempratureValue[0] = value1; 
  TempratureValue[1] = value2;
  TempratureValue[2] = decimal;
 
}
/////////////////////////////////////////////////
void getHumidity(){
  int temp=0,value1,value2;
  for(int i=0;i<sizeof(humidity);i++){
    
    if(humidity[i]==1){// Kollar om det finns en 1 i så fall aderas bit positionens värde
      temp += BitValue[i+3];
           
    }
    
    
  } 
  // Sparar värden i en array
  value2 = temp%10;
  value1 = temp/10;
  HumidityValue[0] = value1;
  HumidityValue[1] = value2;
 
  
  
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void set_clock_serial(void){
  
  char *token;
  
  int DateCount=0,TimeCount=0;
  
  // set the time RTC throw uart
      
      uint8_t text[]= "\n Input Date yyyy-mm-dd:\n\r";
      // printf("I:%d\n",i);
      SendToSerial(text,sizeof(text));
      ReciveFromUser_clk(tempInputDate,10);
      SendToSerial(tempInputDate,sizeof(tempInputDate));
      SendToSerial(NewLine,sizeof(NewLine));
      
      /* get the first token */
      token = strtok(tempInputDate, "-");
      // Delar upp den inkomande Datum till år,månad,dag
      while( token != NULL ) 
      {     
        SetDate[DateCount] = token;      
        DateCount++;    
        token = strtok(NULL, "-");   
        
      }
    
      
    // set the time RTC throw uart
      uint8_t text_h[]= "\n Input Time hh:mm:\n\r";      
      SendToSerial(text_h,sizeof(text_h));      
      ReciveFromUser_clk(tempInputTime,5);
      SendToSerial(tempInputTime,sizeof(tempInputTime));
      SendToSerial(NewLine,sizeof(NewLine));
      
      // Dellar upp det inkomande tiden till enskilda variabler
      token = strtok(tempInputTime, ":");
      while( token != NULL ) 
      {     
        SetTime[TimeCount] = token;        
        TimeCount++;    
        token = strtok(NULL, ":");        
        
      }
    
    
  
  
  RTC_CalendarConfig(); // Kallar på funktionen för att sätta den nya klockan
  
  
  
}
/////////////TIM1 INTERUPTS FÖR PULS BRED///////////////////////////////////////
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
  
  HAL_TIM_IC_Stop_IT(&htim1,TIM_CHANNEL_2);
  if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_2){   
    
    
    uwIC2Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
    if(uwIC2Value1<2000 && uwIC2Value1>400)
    {
      //printf("hej\n");
      BitCount++;
      
      if((400)<uwIC2Value1 && uwIC2Value1 <(1000) && BitCount<5 ){
        PreambleCount++;      
        // printf("Preamble: %d\n",PreambleCount);
      }
      else if(BitCount > PreambleCount && PreambleCount != 4){
        BitCount=0;
        PreambleCount=0;     
      } 
      
      if(PreambleCount== 4){
        RecivedPacket[BitCount-5] = uwIC2Value1;
        // arrayCount++;
        // printf("Array: %d\n",RecivedPacket[BitCount-10]);
        
        
        if(BitCount == 100){
          CalculateTempraturePacket();
          BitCount=0;
          PreambleCount=0;
          //arrayCount=0;
        } 
      }
    }
    
    
  } 
  
  
  HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_2);
}
////////////////////////////////////////////////////////////////////////////////////////////
int CalculatePulsWithd(uint32_t puls){
  
  
  
  if((1200)< puls && puls <(1900) || puls==0)
  {
    return 0;
  }
  else if((400)<puls && puls <(1000) || puls==1){
    return 1;
  }
  
  return -1;
  
}
/////////////////////////////////////////////////////////
void CalculateTempraturePacket(void){
  static uint32_t temp[150];
  static uint32_t packet[40];
  int flag=0;
  
  for(int i=0;i<100;i++){
    temp[i]=CalculatePulsWithd(RecivedPacket[i]);  
    
  }
  
  for(int i=0;i<100;i++){
    if(temp[i]==0 && temp[i+1]==1 && temp[i+2]==0 && temp[i+3]==0 && temp[i+4]==1 && temp[i+5]==0 && flag != 1  )
    {   
     
      
      for(int a=i; a<(i+41); a++)
      {
        packet[a-i]= temp[a];       
      }
      flag = 1;
            
    }
    
  }
 
  // Räknar ut CRC koden 
  CrcCode = HAL_CRC_Calculate(&hcrc, packet, 40);
  
  printf("\nCRC: %d\n",CrcCode);
  
  if(CrcCode == 0 && flag == 1){ // Om CRC värdet är 0 så uppdateras värden 
    
    
    for(int i=14;i<24;i++){ // Sparar de binära värden för tempraturen i en egen Tempratur array
      Temprature[i-14]= packet[i];
     
    }
    
    for(int a=25;a<31;a++){ // Spara de binära värden för luft fuktighet i en egen array
      humidity[a-25]= packet[a];     
    } 
    
    getTempratue();   // Räknar ut decimala värdet för Tempratur 
    getHumidity();  // Räknar ut decimala värdet för luftfuktighet 
    
    
    uint8_t text_Temp[] = "\n\rTemprature is: ";
    SendToSerial(text_Temp,sizeof(text_Temp));
    char Temp[]={TempratureValue[0]+48,TempratureValue[1]+48,44,TempratureValue[2]+48};// Sparar Temprature värdet och addrar en komma 
   
    SendToSerial(Temp,sizeof(Temp)); // Skickar värdet till Uart
    
    
  }
  
  
  
  
}