#include "A101.h"
#include "can.h"

#define CALI_DATA 0.00857142857143     //校准系数


float ZGyroData=0;

//校准
void A101_Cali(void)
{
	uint8_t TxData[8];
	int ii=0;
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef TxMessage;
	TxMessage.StdId = 0x404;
	TxMessage.ExtId = 0x12;
	TxMessage.IDE = 0;			//标准标识符
	TxMessage.RTR = 0;			//数据帧
	TxMessage.DLC = 0x08;
	TxMessage.TransmitGlobalTime = DISABLE;
	TxData[0]=0x00;
	TxData[1]=0x01;
	TxData[2]=0x02;
	TxData[3]=0x03;
	TxData[4]=0x04;
	TxData[5]=0x05;
	TxData[6]=0x06;
	TxData[7]=0x07;
	
	for(ii=0;ii++;ii<100)
	HAL_CAN_AddTxMessage(&hcan2, &TxMessage, TxData, &send_mail_box);

}

void A101_Get_Info(CAN_HandleTypeDef *hcan,CAN_RxHeaderTypeDef rx_message,uint8_t *A101_data)
{
	if(rx_message.StdId==0x401){
		ZGyroData=(float)CALI_DATA*((int32_t)(A101_data[0]<<24)|(int32_t)(A101_data[1]<<16)|(int32_t)(A101_data[2]<<8)|(int32_t)(A101_data[3]));	
	}
}

