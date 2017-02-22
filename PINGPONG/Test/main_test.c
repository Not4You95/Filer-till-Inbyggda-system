/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdint.h"
#include "stm32f3xx_hal.h"
#include "gpio.h"
#include "stdbool.h"


typedef enum{
  Start,
  MoveRight,
  MoveLeft} states;
  states State, NextState;

void SystemClock_Config(void);
void Error_Handler(void);




//////////////////////////////////////////////////////////////////////////////////////
int main(void)
{ 
  //////////////////////////////////////////////////////

  
  
  
  
  ////////////////////////////////////////////////////////

  bool ButtenPredded,R_last=false,L_last=false,Lose=false;

uint32_t Varv,Speed,Velocity=0;
uint8_t Led;
int Squre_L=0,Squre_R=0;


State = Start;
NextState = State;
Speed = 500000;
  
  
  
  //////////////////////////////////////////////////////////////////////
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

 /////////////////////////////////////////////////////////////////////////
  while (1)
  {
  State = NextState;
  printf("Led: %d %d %d\n",Led,Velocity,Varv);
  switch(State){
    case Start:  
    
          Led_on(0,0);
          

        if(Lose == true || Squre_L == 4 || Squre_R==4){
            Blink_LED();
            show_points(Squre_L,Squre_R);
            Lose = false; 
            if(Squre_L == 4 || Squre_R == 4){
              Squre_L = 0;
              Squre_R = 0;   
              Velocity = 60;
            }
               
            }
                    
         else if (L_hit() == true && L_last != true)
          {
            Led = 1;
            NextState = MoveRight;             
            L_last=true;            
            R_last=false;
            while(L_hit() == true);
            
          }
          else if(R_hit() == true && R_last != true)
          {
            Led=8;
            NextState = MoveLeft;             
            L_last=false;           
            R_last=true;
            while(R_hit() == true);
            
           }
          else {
            NextState = Start;
          }
    
          break;
          
    case MoveRight:
     
      Led_on(Led,Velocity);     
      Varv = Speed;
     
      while(Varv != 0)
      {
        if(R_hit() ) ButtenPredded  = true;
        Varv--;
       
      }
      if(ButtenPredded){
        if(Led == 8){          
          Led=7;                  
         NextState = MoveLeft;
         Speed -=100;
        // Velocity -= 5;
               }
        else{
        Squre_L++;  
        Lose =true;
        NextState = Start;
        
        }
      
    }
    else
    {
      if(Led == 9){
        NextState = Start;
        Squre_L++;   
        Lose =true;
        
      }
       else
         NextState  = MoveRight;
    }
    if(!ButtenPredded) Led++;
       ButtenPredded = false;            
          
      break;
      
    case MoveLeft:
    {     
     Led_on(Led,Velocity);     
      Varv = Speed;       
      while( Varv !=0){
        if(L_hit() ) ButtenPredded = true;
        Varv--;   
       // HAL_Delay(1);
      }
      
      if(ButtenPredded)
      {
            if(Led == 1){
              NextState = MoveRight;
              Led=2; 
              Speed -=100;
              //Velocity -= 5;
            }
           else{
              NextState = Start;
              Squre_R++;       
              Lose =true;
            }
      }
     else{
         
       if(Led == 0){
            NextState = Start;
            Squre_R++;       
            Lose =true;
     }
          else
            NextState = MoveLeft;
        }
      
        if(!ButtenPredded) Led--;
        ButtenPredded=false;
      }
      break;
      
    default:
      break;
      
   
    

  }
   
    

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
    HAL_GPIO_WritePin(Led_1_GPIO_Port,Led_1_Pin,GPIO_PIN_SET);// LED 1
    HAL_GPIO_WritePin(Led_1_GPIO_Port,Led_1_Pin,GPIO_PIN_RESET);
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
