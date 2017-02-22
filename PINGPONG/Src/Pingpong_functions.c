#include "stdint.h"
#include "gpio.h"
#include "stm32f3xx_hal.h"
#include "stdbool.h"


void Led_on(uint8_t Lednr){
    
  uint8_t i;
  
  for(i=1; i<=8; i++){
    
      switch(i){
        case 1: 
            if(Lednr == 1) HAL_GPIO_WritePin(Led_1_GPIO_Port,Led_1_Pin,GPIO_PIN_SET);
            else HAL_GPIO_WritePin(Led_1_GPIO_Port,Led_1_Pin, GPIO_PIN_RESET);
            break;
            
            
        case 2: if(Lednr==2) HAL_GPIO_WritePin(LED_2_GPIO_Port,LED_2_Pin,GPIO_PIN_SET);// LED 2
                 else HAL_GPIO_WritePin(LED_2_GPIO_Port,LED_2_Pin,GPIO_PIN_RESET);
                 break;
    
        case 3: if(Lednr == 3) HAL_GPIO_WritePin(LED_3_GPIO_Port,LED_3_Pin,GPIO_PIN_SET);// LED 3
                else HAL_GPIO_WritePin(LED_3_GPIO_Port,LED_3_Pin,GPIO_PIN_RESET);
                break;
    
        case 4: if(Lednr == 4) HAL_GPIO_WritePin(LED_4_GPIO_Port,LED_4_Pin,GPIO_PIN_SET);// LED 4
                else HAL_GPIO_WritePin(LED_4_GPIO_Port,LED_4_Pin,GPIO_PIN_RESET);
                break;
    
        case 5: if(Lednr == 5) HAL_GPIO_WritePin(LED_5_GPIO_Port,LED_5_Pin,GPIO_PIN_SET);// LED 5
                 else HAL_GPIO_WritePin(LED_5_GPIO_Port,LED_5_Pin,GPIO_PIN_RESET);
                 break;
    
        case 6: if(Lednr == 6) HAL_GPIO_WritePin(LED_6_GPIO_Port,LED_6_Pin,GPIO_PIN_SET);// LED 6
                else HAL_GPIO_WritePin(LED_6_GPIO_Port,LED_6_Pin,GPIO_PIN_RESET);
                break; 
    
        case 7: if(Lednr == 7) HAL_GPIO_WritePin(LED_7_GPIO_Port,LED_7_Pin,GPIO_PIN_SET);// LED 7
                  else HAL_GPIO_WritePin(LED_7_GPIO_Port,LED_7_Pin,GPIO_PIN_RESET);
                  break;
                  
        case 8: if(Lednr == 8) HAL_GPIO_WritePin(LED_8_GPIO_Port,LED_8_Pin,GPIO_PIN_SET);// LED 8
                else HAL_GPIO_WritePin(LED_8_GPIO_Port,LED_8_Pin,GPIO_PIN_RESET);
                     break;
        default:
        ;
        
      }
    }
  return;  
}

////////////////////SHOW points//////////////////////////////////////////////////////////////////////////////
void show_points(uint8_t L_points, uint8_t R_points){
   printf("\nScore:\nL-points: %d\nR-points: %d \n",L_points,R_points);
  for(int i=1; i<=8; i++){   
      switch(i){
        case 1: 
            if(L_points >= 1) HAL_GPIO_WritePin(Led_1_GPIO_Port,Led_1_Pin,GPIO_PIN_SET);
            else HAL_GPIO_WritePin(Led_1_GPIO_Port,Led_1_Pin, GPIO_PIN_RESET);           
            
            
        case 2: if(L_points >=2) HAL_GPIO_WritePin(LED_2_GPIO_Port,LED_2_Pin,GPIO_PIN_SET);// LED 2
                 else HAL_GPIO_WritePin(LED_2_GPIO_Port,LED_2_Pin,GPIO_PIN_RESET);
                 
    
        case 3: if(L_points >= 3) HAL_GPIO_WritePin(LED_3_GPIO_Port,LED_3_Pin,GPIO_PIN_SET);// LED 3
                else HAL_GPIO_WritePin(LED_3_GPIO_Port,LED_3_Pin,GPIO_PIN_RESET);
                
    
        case 4: if(L_points >= 4) HAL_GPIO_WritePin(LED_4_GPIO_Port,LED_4_Pin,GPIO_PIN_SET);// LED 4
                else HAL_GPIO_WritePin(LED_4_GPIO_Port,LED_4_Pin,GPIO_PIN_RESET);
                
    
        case 5: if(R_points >= 4) HAL_GPIO_WritePin(LED_5_GPIO_Port,LED_5_Pin,GPIO_PIN_SET);// LED 5
                 else HAL_GPIO_WritePin(LED_5_GPIO_Port,LED_5_Pin,GPIO_PIN_RESET);
                 
    
        case 6: if(R_points >= 3) HAL_GPIO_WritePin(LED_6_GPIO_Port,LED_6_Pin,GPIO_PIN_SET);// LED 6
                else HAL_GPIO_WritePin(LED_6_GPIO_Port,LED_6_Pin,GPIO_PIN_RESET);
                 
    
        case 7: if(R_points >= 2) HAL_GPIO_WritePin(LED_7_GPIO_Port,LED_7_Pin,GPIO_PIN_SET);// LED 7
                  else HAL_GPIO_WritePin(LED_7_GPIO_Port,LED_7_Pin,GPIO_PIN_RESET);
                  
                  
        case 8: if(R_points >= 1) HAL_GPIO_WritePin(LED_8_GPIO_Port,LED_8_Pin,GPIO_PIN_SET);// LED 8
                else HAL_GPIO_WritePin(LED_8_GPIO_Port,LED_8_Pin,GPIO_PIN_RESET);
                     
        default:
        ;
        
      }
    }
  
  HAL_Delay(500);
  return;
}
///////////////////Butten Right//////////////////////////////////////////////////////////////////////////////////////////

bool R_hit(void){
  while(HAL_GPIO_ReadPin(R_BUTTEN_GPIO_Port,R_BUTTEN_Pin) !=0 ){
            //printf("\nfalse");
            return false; 
  }
  
  while(HAL_GPIO_ReadPin(R_BUTTEN_GPIO_Port,R_BUTTEN_Pin) == 0 ){
    //printf("\ntrue");
    return true;
    
  }
            
  
}
//////////////////////////Butten Left/////////////////////////////////////////////////////////////////////////////////////
bool L_hit(void){
  while(HAL_GPIO_ReadPin(L_BUTTEN_GPIO_Port,L_BUTTEN_Pin) !=0 ){
           // printf("\nfalse");
            return false; 
  }
  
  while(HAL_GPIO_ReadPin(L_BUTTEN_GPIO_Port,L_BUTTEN_Pin) == 0 ){
    //printf("\ntrue");
    return true;
    
  }
            
  
}
///////////////////////////////////////////////////////////////////////////////////
void Blink_LED(void){
              /////////////////////ON-lED/////////////////////////////////////
                  HAL_GPIO_WritePin(Led_1_GPIO_Port,Led_1_Pin,GPIO_PIN_SET);       
                  HAL_GPIO_WritePin(LED_2_GPIO_Port,LED_2_Pin,GPIO_PIN_SET);// LED 2                
                  HAL_GPIO_WritePin(LED_3_GPIO_Port,LED_3_Pin,GPIO_PIN_SET);// LED 3
                  HAL_GPIO_WritePin(LED_4_GPIO_Port,LED_4_Pin,GPIO_PIN_SET);// LED 4
                  HAL_GPIO_WritePin(LED_5_GPIO_Port,LED_5_Pin,GPIO_PIN_SET);// LED 5
                  HAL_GPIO_WritePin(LED_6_GPIO_Port,LED_6_Pin,GPIO_PIN_SET);// LED 6
                  HAL_GPIO_WritePin(LED_7_GPIO_Port,LED_7_Pin,GPIO_PIN_SET);// LED 7
                 HAL_GPIO_WritePin(LED_8_GPIO_Port,LED_8_Pin,GPIO_PIN_SET);// LED 8
                  
                  HAL_Delay(500);
        ///////////////////////OFF//////////////////////////////////////////////
                  HAL_GPIO_WritePin(Led_1_GPIO_Port,Led_1_Pin, GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(LED_2_GPIO_Port,LED_2_Pin,GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(LED_3_GPIO_Port,LED_3_Pin,GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(LED_4_GPIO_Port,LED_4_Pin,GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(LED_5_GPIO_Port,LED_5_Pin,GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(LED_6_GPIO_Port,LED_6_Pin,GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(LED_7_GPIO_Port,LED_7_Pin,GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(LED_8_GPIO_Port,LED_8_Pin,GPIO_PIN_RESET);
  
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////