#include "motor.h"
#include "struct_typedef.h"

#include "cmsis_os.h"

#include "main.h"
#include "bsp_rng.h"
#include "chassis.h"
#include "gimbal.h"
#include "ammunition_feed.h"
#include "can.h"
#include "detect_task.h"
#include "shoot_fric.h"
//#include "BMI088.h"

u8 Can1_ReceiveBuffer[8];		//can1接收缓冲区
u8 Can2_ReceiveBuffer[8];		//can2接收缓冲区

/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
//    CAN_RxHeaderTypeDef rx_header;
//    uint8_t rx_data[8];
//    
//    if(hcan == &hcan1) {
//        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
//        
//        // 达妙4310反馈处理 (CAN1)
//        if(rx_header.StdId == 0x001) {
//            Get_Gimbal_Motor_Info(rx_header.StdId, rx_data);
//        }
//        // 大疆电机反馈处理
//        else if(rx_header.StdId - 0x205 >= 0 && rx_header.StdId - 0x205 < 2) {
//            Get_Gimbal_Motor_Info(rx_header.StdId, rx_data);
//        }
//    }
//    
//    if(hcan == &hcan2) {
//        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
//        
//        // 大疆yaw轴反馈处理 (CAN2)
//        if(rx_header.StdId - 0x205 >= 0 && rx_header.StdId - 0x205 < 2) {
//            Get_Gimbal_Motor_Info(rx_header.StdId, rx_data);
//        }
//    }
//}
//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//	    CAN_RxHeaderTypeDef rx_header;
//    CAN_RxHeaderTypeDef rx_header1,rx_header2,rx_header3;
//    uint8_t rx_data[8],rx_data1[8],rx_data2[8],rx_data3[8];
//
//	if(hcan == &hcan1){
//
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header1, rx_data1);
//
//		static uint8_t i = 0;
////            //get motor id
//		for( i = 0; i < rx_header1.DLC; ++i)
//		{
//			Can1_ReceiveBuffer[i] = rx_data1[i];
//		}
//	//从缓冲区提取信息
//    Get_Gimbal_Motor_Info(rx_header.StdId, rx_data);
////		Get_Chassis_Motor_Info( rx_header1.StdId, Can1_ReceiveBuffer);
//		//Get_UWB_Info( RxMessage.StdId, Can1_ReceiveBuffer);
//		Get_Ammunition_Motor_Info( rx_header1.StdId, Can1_ReceiveBuffer);
////		if(rx_header3.StdId=0x211)
////		{	Can1_ReceiveBuffer[8] = rx_data3[8];
//		Get_Chaojidianrong(rx_header1.StdId,Can1_ReceiveBuffer);
//
//	}
//
//	if(hcan == &hcan2){
//
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header2, rx_data2);
//
//		static uint8_t i = 0;
////            //get motor id
//		for( i = 0; i < rx_header2.DLC; ++i)
//		{
//			Can2_ReceiveBuffer[i] = rx_data2[i];
//		}
//	//从缓冲区提取信息
//		Get_Chassis_Motor_Info( rx_header2.StdId, Can2_ReceiveBuffer);
//		Get_Gimbal_Motor_Info( rx_header2.StdId, Can2_ReceiveBuffer);
//		Get_Shoot_Motor_Info( rx_header2.StdId, Can2_ReceiveBuffer);
//	}
//}

//CAN1发送函数（标准ID）
void CAN1_Send_Msg( uint32_t stdid, u8 *msg, u8 len)
{
//	u8 mbox;
	u16 i = 0;
	u8 TxData[8];
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef TxMessage;
	TxMessage.StdId = stdid;
	TxMessage.ExtId = 0x12;
	TxMessage.IDE = 0;			//标准标识符
	TxMessage.RTR = 0;			//数据帧
	TxMessage.DLC = len;
	
	for( i = 0; i < len; ++i)
		TxData[i] = msg[i];
	HAL_CAN_AddTxMessage(&hcan1, &TxMessage, TxData, &send_mail_box);
}

void CAN2_Send_Msg( uint32_t stdid, u8 *msg, u8 len)
{

	u16 i = 0;
	u8 TxData[8];
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef TxMessage;
	TxMessage.StdId = stdid;
	TxMessage.ExtId = 0x12;
	TxMessage.IDE = 0;			//标准标识符
	TxMessage.RTR = 0;			//数据帧
	TxMessage.DLC = len;
	
	for( i = 0; i < len; ++i)
		TxData[i] = msg[i];
	HAL_CAN_AddTxMessage(&hcan2, &TxMessage, TxData, &send_mail_box);
}

