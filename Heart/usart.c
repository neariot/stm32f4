
#include "stm32f4xx.h"
#include "usart.h"



void usart_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO���Խṹ������  ���ÿ⺯�� 
	USART_InitTypeDef USART_InitStructure;//�������Խṹ������
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//��������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//��������ʱ��
	
	USART_DeInit(USART3);//��������
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3);//GPIO��������
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); 
	
	// GPIO CONFIG
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //ģʽΪ��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//������� ����
 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����ʱ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶȵȼ� ����
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO
	
	//USART CONFIG
	USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֽڿ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //
	
	
	NVIC_Config();  //�ж�����
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  //enable  receive interrupt
	
	USART_Cmd(USART3, ENABLE);//ʹ�ܴ���
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
	while(!((USART3->SR)&(1<<7)));//�ȴ�������
	USART3->DR=byte; //����һ���ֽ�
}

/*SEND A BUFFER*/
void UART_Send(uint8_t *Buffer, uint32_t Length)
{
	while(Length != 0)
	{
		while(!((USART3->SR)&(1<<7)));//�ȴ�������
		USART3->DR= *Buffer;
		Buffer++;
		Length--;
	}
}

/*receive one byte*/
uint8_t UART_Recive(void)
{
	while(!(USART3->SR & (1<<5)));//�ȴ����յ�������
	return(USART3->DR); //��������
}
