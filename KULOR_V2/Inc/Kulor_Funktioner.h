void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);
void SendToSerial(uint8_t *text, uint8_t size);
void ReciveFromUser_clk(char *temp,uint8_t size);
void assert_failed(uint8_t *file, uint32_t line);
void RTC_CLOCK_SETINGS(void);
void RTC_CalendarShow();
//void RTC_test(void);
void RTC_CalendarConfig(void);

void UppDateDisplay(uint8_t number);
void SetDisplayTemp(uint8_t number[]);
void getTempratue();
void getHumidity();



void test(void);
void CalculateTempraturePacket(void);
int CalculatePulsWithd(uint32_t);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

/////////////////////////////////////////////
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */



