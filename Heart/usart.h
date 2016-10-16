

#include "stm32f4xx.h"

void usart_config(void);
void UART_send_byte(uint8_t byte);
void UART_Send(uint8_t *Buffer, uint32_t Length);
uint8_t UART_Recive(void);
void NVIC_Config(void);
