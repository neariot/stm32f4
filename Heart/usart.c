
#include "stm32f4xx.h"
#include "usart.h"



void usart_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO属性结构体声明  调用库函数 
	USART_InitTypeDef USART_InitStructure;//串口属性结构体声明
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//开启引脚时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//开启外设时钟
	
	USART_DeInit(USART3);//串口重置
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3);//GPIO复用配置
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); 
	
	// GPIO CONFIG
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//GPIO引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //模式为复用外设
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//输出类型 推挽
 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//空闲时上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度等级 中速
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
	
	//USART CONFIG
	USART_InitStructure.USART_BaudRate = 115200;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字节宽度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_Init(USART3, &USART_InitStructure); //
	
	
	NVIC_Config();  //中断配置
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  //enable  receive interrupt
	
	USART_Cmd(USART3, ENABLE);//使能串口
}


void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}
/*SEND ONE BYTE*/
void UART_send_byte(uint8_t byte) //
{
	while(!((USART3->SR)&(1<<7)));//等待发送完
	USART3->DR=byte; //发送一个字节
}

/*SEND A BUFFER*/
void UART_Send(uint8_t *Buffer, uint32_t Length)
{
	while(Length != 0)
	{
		while(!((USART3->SR)&(1<<7)));//等待发送完
		USART3->DR= *Buffer;
		Buffer++;
		Length--;
	}
}

/*receive one byte*/
uint8_t UART_Recive(void)
{
	while(!(USART3->SR & (1<<5)));//等待接收到的数据
	return(USART3->DR); //读出数据
}
