#include "main.h"
#include "stm32f4_discovery.h"
#include "tm_stm32f4_adc.h"

uint32_t usart_cnt;	
uint32_t timer_cnt;
extern int IBI;          //相邻节拍时间
extern int BPM;          //心率值             
extern int Signal;       //原始信号值            
extern unsigned char QS; //发现心跳标志
//extern int count;
uint32_t usart_cnt;	
uint32_t timer_cnt;


int main(void)
{
  /* Initialize Leds mounted on STM32F4-Discovery board */
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);

  /* Turn on LED4 and LED5 */
  STM_EVAL_LEDOn(LED4);
  STM_EVAL_LEDOn(LED5);
		
	TM_ADC_Init(ADC1, ADC_Channel_0);
	
	usart_config();
	USART_ClearFlag(USART3,USART_FLAG_TC);
	TIM3_Config();
	TIM4_Config();
	printf("\n\r>> !! USART IS SETED. !! <<\n\r");
	printf("\n\r>> !! USART IS OK. !! <<\n\r");
	//printf("%d",timer_cnt);
		
	while (1)
  {	
		if(timer_cnt==1)
		{
      usart_cnt++;
			UART_send_byte(HIBYTE(usart_cnt));
			UART_send_byte(LOBYTE(usart_cnt));
      //printf("\n\r>> !! USART IS OK. !! <<\n\r");		
			STM_EVAL_LEDToggle(LED3);
			STM_EVAL_LEDToggle(LED6);
			timer_cnt=0;
			printf("Signal: %4d\n\r", Signal);
			//printf("BPM: %4d\n\r", TM_ADC_Read(ADC1, ADC_Channel_0));
			printf("BPM: %4d\n\r", BPM);
		  //printf("Count: %4d\n\r", count);
			//printf("%4d\n\r", TM_ADC_Read(ADC1, ADC_Channel_0));
      //printf("\n\r>> !! USART IS OK. !! <<\n\r");
      }
			//delay(2000);	//delay for 20ms
    }
}
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length.
  * @retval None
  */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */
#ifdef __GNUC__ 
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf 
     set to 'Yes') calls __io_putchar() */ 
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch) 
#else 
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f) 
#endif /* __GNUC__ */ 
PUTCHAR_PROTOTYPE 
{ 
	/* Place your implementation of fputc here */ 
	/* e.g. write a character to the USART */ 
	USART_SendData(USART3, (uint8_t) ch); 
	/* Loop until the end of transmission */ 
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
	return ch; 
}

//static void delay(uint32_t time)
//{
//  uint32_t i,j;

//  for(i=0;i<time;++i)
//  {
//    for(j=0;j<10000;++j)
//    {       
//    }
//  }
//}



