/**
*****************************************************************************
**
**  File        : stm32f4xx_it.c
**
**  Abstract    : Main Interrupt Service Routines.
**                This file provides template for all exceptions handler and
**                peripherals interrupt service routine.
**
**  Environment : Atollic TrueSTUDIO(R)
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
**  Distribution: The file is distributed 揳s is,?without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the Atollic TrueSTUDIO(R) toolchain.
**
**
*****************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"
#include "tm_stm32f4_adc.h"
extern uint32_t timer_cnt;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

 
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/



/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}




//	__asm void wait()
//{
//}
///**
//  * @brief  This function handles Hard Fault exception.
//  * @param  None
//  * @retval None
//  */
//void HardFault_Handler(void)
//{
//  /* Go to infinite loop when Hard Fault exception occurs */
//	wait();
////   while (1)
////   {
////   }
//}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void)
{

}

/**
  * @brief  This function handles OTG_FS_WKUP_IRQ Handler.
  * @param  None
  * @retval None
  */

/**
  * @brief  This function handles OTG_HS_WKUP_IRQ Handler.
  * @param  None
  * @retval None
  */

/**
  * @brief  This function handles OTG_xx_IRQ Handler.
  * @param  None
  * @retval None
  */


/**
  * @brief  This function handles EP1_IN Handler.
  * @param  None
  * @retval None
  */


/**
  * @brief  This function handles EP1_OUT Handler.
  * @param  None
  * @retval None
  */


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
int BPM;                   // used to hold the pulse rate
int Signal;                // holds the incoming raw data
int IBI = 600;             // holds the time between beats, must be seeded! 
unsigned char Pulse = false;     // true when pulse wave is high, false when it's low
unsigned char QS = false;
int rate[10];                    // array to hold last ten IBI values
unsigned long sampleCounter = 0;          // used to determine pulse timing
unsigned long lastBeatTime = 0;           // used to find IBI
int P =512;                      // used to find peak in pulse wave, seeded
int T = 512;                     // used to find trough in pulse wave, seeded
int thresh = 512;                // used to find instant moment of heart beat, seeded
int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
unsigned char firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
unsigned char secondBeat = false;      // used to seed rate array so we startup with reasonable BPM
//int count;
void TIM3_IRQHandler(void)
{
	uint16_t runningTotal=0;
	uint8_t i;
	uint16_t Num;
	
	//printf("\n\r>> !! USART IS OK. !! <<\n\r");
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		//count++;
		//读取到的值右移2位，12位-->10位
		//BPM=BPM+1;
		Signal = TM_ADC_Read(ADC1, ADC_Channel_0)>>2;//ADC_GetConversionValue(ADC1)>>2;     // read the Pulse Senso
		//Signal=Get_Adc_Average(ADC_Channel_1,1)>>2; 
		sampleCounter += 2;                          // keep track of the time in mS with this variable
		Num = sampleCounter - lastBeatTime;          // monitor the time since the last beat to avoid noise	
		
		//发现脉冲波的波峰和波谷
		//  find the peak and trough of the pulse wave
		if(Signal < thresh && Num > (IBI/5)*3){   // avoid dichrotic noise by waiting 3/5 of last IBI
			if (Signal < T){                        // T is the trough
				T = Signal;                           // keep track of lowest point in pulse wave 
			}
		}

		if(Signal > thresh && Signal > P){        // thresh condition helps avoid noise
			P = Signal;                             // P is the peak
		}                                         // keep track of highest point in pulse wave

  //开始寻找心跳
	//当脉冲来临的时候，signal的值会上升
	//  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (Num > 250){                                 // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (Num > (IBI/5)*3) ){        
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
    
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat = false;                  // clear secondBeat flag
        for(i=0; i<=9; i++){                 // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;                     //rate[0]-rate[9]的值都一样吗？？？？
        }
      }

      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
        return;                              // IBI value is unreliable so discard it
      }   

      // keep a running total of the last 10 IBI values
     // runningTotal = 0;                  		// clear the runningTotal variable    

      for(i=0; i<=8; i++){                    // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value 
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = true;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

	//脉冲开始下降
  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over														 //灯灭
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  //没有检测到脉冲，设置默认值
	if (Num > 2500){                         // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
  }

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void USART3_IRQHandler(void) 
{
		   if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	     {
				 USART_ReceiveData(USART3);
		     USART_ClearITPendingBit(USART3,USART_IT_RXNE); 
	     }
			 if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)  //如果发生溢出，先读SR,再读DR寄存器，即可清除不断入中断的问题
		   { 
			  USART_ClearFlag(USART3,USART_FLAG_ORE); 
			  USART_ReceiveData(USART3);
		    }
}


void TIM4_IRQHandler(void)
{
	uint16_t capture=0;
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		  capture = TIM_GetCapture1(TIM4);
			TIM_SetCompare1(TIM4, capture + 50000);
		}
		timer_cnt = timer_cnt + 1;
}










/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


