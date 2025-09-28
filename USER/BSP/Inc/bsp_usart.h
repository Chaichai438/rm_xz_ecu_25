#ifndef BSP_USART_H
#define BSP_USART_H
#include "struct_typedef.h"


#define JUDGE_DMA_RXBUF_SIZE 	512
#define VISION_DMA_RXBUF_SIZE	8
#define IMU_DMA_RXBUF_SIZE	  20


extern unsigned char vision_dma_rxbuf[VISION_DMA_RXBUF_SIZE];
extern int VISION_CNT;
extern void usart6_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
void usart1_Init(void);
extern void usart1_tx_dma_init(void);
extern void usart1_tx_dma_enable(uint8_t *data, uint16_t len);
void vision_data_get(void);
void vision_data_UARTget(uint8_t *RxBuffer);
extern volatile float PITCH_VISION,YAW_VISION,PITCH_VISION_LAST,YAW_VISION_LAST;
#endif
