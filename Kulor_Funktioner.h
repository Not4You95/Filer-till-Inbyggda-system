void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);
void SendToSerial(uint8_t *text, uint8_t size);
void ReciveFromUser_clk(char *temp,uint8_t size);
void assert_failed(uint8_t *file, uint32_t line);
void RTC_CLOCK_SETINGS(void);
void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate);
//void RTC_test(void);
void RTC_CalendarConfig(void);
void SetLed(void);
void test(void);
//void RTC_CalendarConfig_test(void);



