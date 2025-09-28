#include "bsp_usart.h"
#include "main.h"
#include "bsp_buzzer.h"
#include "detect_task.h"
#include <string.h>
#include "usb_task.h"
#include "st_version.h"

#define RX_BUFFER_SIZE  12

volatile float PITCH_VISION,YAW_VISION,PITCH_VISION_LAST,YAW_VISION_LAST;
unsigned char vision_dma_rxbuf[VISION_DMA_RXBUF_SIZE];
unsigned char RxBuffer[20];


extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;

void vision_data_UARTget(uint8_t *RxBuffer);

void usart1_tx_dma_init(void)
{

    //enable the DMA transfer for the receiver and tramsmit request
    //使能DMA串口接收和发送
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAT);

    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart1_tx);

    while(hdma_usart1_tx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart1_tx);
    }

    hdma_usart1_tx.Instance->PAR = (uint32_t) & (USART1->DR);
    hdma_usart1_tx.Instance->M0AR = (uint32_t)(NULL);
    hdma_usart1_tx.Instance->NDTR = 0;


}
void usart1_tx_dma_enable(uint8_t *data, uint16_t len)
{
    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart1_tx);

    while(hdma_usart1_tx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart1_tx);
    }

    __HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, DMA_HISR_TCIF7);

    hdma_usart1_tx.Instance->M0AR = (uint32_t)(data);
    __HAL_DMA_SET_COUNTER(&hdma_usart1_tx, len);

    __HAL_DMA_ENABLE(&hdma_usart1_tx);
}



void usart6_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{

    //enable the DMA transfer for the receiver and tramsmit request
    //使能DMA串口接收和发送
			SET_BIT(huart6.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart6.Instance->CR3, USART_CR3_DMAT);

    //enalbe idle interrupt
    //使能空闲中断
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);



    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart6_rx);
    
    while(hdma_usart6_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart6_rx);
    }

    __HAL_DMA_CLEAR_FLAG(&hdma_usart6_rx, DMA_LISR_TCIF1);

    hdma_usart6_rx.Instance->PAR = (uint32_t) & (USART6->DR);
    //memory buffer 1
    //内存缓冲区1
    hdma_usart6_rx.Instance->M0AR = (uint32_t)(rx1_buf);
    //memory buffer 2
    //内存缓冲区2
    hdma_usart6_rx.Instance->M1AR = (uint32_t)(rx2_buf);
    //data length
    //数据长度
    __HAL_DMA_SET_COUNTER(&hdma_usart6_rx, dma_buf_num);

    //enable double memory buffer
    //使能双缓冲区
    SET_BIT(hdma_usart6_rx.Instance->CR, DMA_SxCR_DBM);

    //enable DMA
    //使能DMA
    __HAL_DMA_ENABLE(&hdma_usart6_rx);


    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart6_tx);

    while(hdma_usart6_tx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart6_tx);
    }

    hdma_usart6_tx.Instance->PAR = (uint32_t) & (USART6->DR);

}



void usart6_tx_dma_enable(uint8_t *data, uint16_t len)
{
    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart6_tx);

    while(hdma_usart6_tx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart6_tx);
    }

    __HAL_DMA_CLEAR_FLAG(&hdma_usart6_tx, DMA_HISR_TCIF6);

    hdma_usart6_tx.Instance->M0AR = (uint32_t)(data);
    __HAL_DMA_SET_COUNTER(&hdma_usart6_tx, len);

    __HAL_DMA_ENABLE(&hdma_usart6_tx);
}


void usart1_init(uint8_t *rx1_buf,  uint16_t dma_buf_num)
{

    //enable the DMA transfer for the receiver and tramsmit request
    //使能DMA串口接收和发送
    SET_BIT(huart6.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart6.Instance->CR3, USART_CR3_DMAT);

    //enalbe idle interrupt
    //使能空闲中断
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);



    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart6_rx);
    
    while(hdma_usart6_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart6_rx);
    }

    __HAL_DMA_CLEAR_FLAG(&hdma_usart6_rx, DMA_LISR_TCIF1);

    hdma_usart6_rx.Instance->PAR = (uint32_t) & (USART6->DR);
    //memory buffer 1
    //内存缓冲区1
    hdma_usart6_rx.Instance->M0AR = (uint32_t)(rx1_buf);
//    //memory buffer 2
//    //内存缓冲区2
//    hdma_usart6_rx.Instance->M1AR = (uint32_t)(rx2_buf);
    //data length
    //数据长度
    __HAL_DMA_SET_COUNTER(&hdma_usart6_rx, dma_buf_num);

    //enable double memory buffer
    //使能双缓冲区
    SET_BIT(hdma_usart6_rx.Instance->CR, DMA_SxCR_DBM);

    //enable DMA
    //使能DMA
    __HAL_DMA_ENABLE(&hdma_usart6_rx);


    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart6_tx);

    while(hdma_usart6_tx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart6_tx);
    }

    hdma_usart6_tx.Instance->PAR = (uint32_t) & (USART6->DR);

}

int RX_FLAG=0;

//使用串口空闲中断
void USART1_IRQHandler(void)
{
	static uint8_t res = 0,rxConut=0;
	if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE) != RESET)
	{
		//HAL_UART_Receive(&huart1,&res,1,0x1fff);
		res=huart1.Instance->DR;
		if(!RX_FLAG){
			RxBuffer[rxConut++]=res;
			if(rxConut==RX_BUFFER_SIZE)
				RX_FLAG=1;		
		}
		__HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_RXNE);
	}

	if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) != RESET)  //接收到一帧数据
	{		
		//while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC) != SET);
	
		//HAL_UART_Transmit(&UART1_HandleStructure,pData,len,1000);
		//发
		res=huart1.Instance->DR;
		RX_FLAG=0;
		vision_data_UARTget(RxBuffer);
		rxConut = 0;
		//__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		__HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_IDLE);
	}
}


void usart1_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);//接收中断
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);//空闲中断
}

uint8_t vision_buffer[12];
int vision_flag=0;
int VISION_CNT=0;
int buff_fire=0;
float last_yaw_notzero=0;
extern int distinguish;
extern int buff_auto_status;
union shijue_union pitch_vision,yaw_vision;
void vision_data_UARTget(uint8_t *RxBuffer)                        //读取视觉
{
	int i,yaw,pitch,fire;
 	yaw = 0;
	pitch = 0;
	PITCH_VISION_LAST=PITCH_VISION;	
	vision_flag=1;													//			vision_flag=1		
	VISION_CNT++;														//	VISION_CNT=1
	for(i=0;i<RX_BUFFER_SIZE;i++)
	{
		vision_buffer[i] = RxBuffer[i];								//读取信息
	}
	if( vision_buffer[0] == 0xED && vision_buffer[11] == 0xEC)							//首位0xED 末尾0xEC
	{	
		for(int i=0;i<4;i++)
		{
			yaw_vision.b[i]=vision_buffer[i+1];//yaw   1 2 3 4 位
			pitch_vision.b[i]=vision_buffer[i+5];//pitch 5 6 7 8 位
		}
		if(vision_buffer[9]==0x77){
				buzzer_on(95, 10000); 
				distinguish=1;//5位检测到0x77 蜂鸣器开
		}
		else if(vision_buffer[9]==0x88){
				buzzer_on(95, 10000); 
				distinguish=3;//5位检测到0x77 蜂鸣器开
		}
		else
		{
			buzzer_on(10,2000);
			distinguish=0;
		}
		if(vision_buffer[10]==0x55){
			buff_auto_status=1;																								//5位检测到0x55 1 风车模式
		}
		if(vision_buffer[10]==0x00){
			buff_auto_status=0;																								//5位检测到0x00 0 装甲板模式
		}
//		if(buff_auto_status==1&&vision_buffer[6]==0x11)
//			buff_fire=40;																											//风车模式且6位0x11   buff_fire在射击control函数里用为判断
		
		PITCH_VISION=pitch_vision.a;
		YAW_VISION=yaw_vision.a;																								//pitch yaw数据处理
		last_yaw_notzero=YAW_VISION;																				//测算出的YAW	赋给last_yaw_notzero
	}	
	else 																																	//如果没收到 
	{
		buzzer_off();
		PITCH_VISION=0.0f;
		YAW_VISION=last_yaw_notzero;                                       //蜂鸣器关，PITCH=0，YAW_VISION等于上一次的last_yaw_notzero 
	}
}



