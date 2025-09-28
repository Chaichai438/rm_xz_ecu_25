/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       can_receive.c/h
  * @brief      there is CAN interrupt function  to receive motor data,
  *             and CAN send function to send motor current to control motor.
  *             这里是CAN中断接收函数，接收电机数据,CAN发送函数发送电机电流控制电机.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *  V1.1.0     Nov-11-2019     RM              1. support hal lib
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include "CAN_receive.h"
#include "Motor_ALL.h"
#include "cmsis_os.h"


#include "main.h"
#include "bsp_rng.h"
#include "A101.h"

#include "detect_task.h"
short round_cnt;
int i=0;
short diff;
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;


//motor data read
#define get_motor_measure(ptr, data)                                    \
    {                                                                   \
        (ptr)->last_ecd = (ptr)->ecd;                                   \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (data)[6];                                   \
		                                       		\
    }
/*
motor data,  0:chassis motor1 3508;1:chassis motor3 3508;2:chassis motor3 3508;3:chassis motor4 3508;
4:yaw gimbal motor 6020;5:pitch gimbal motor 6020;6:trigger motor 2006;
电机数据, 0:底盘电机1 3508电机,  1:底盘电机2 3508电机,2:底盘电机3 3508电机,3:底盘电机4 3508电机;
4:yaw云台电机 6020电机; 5:pitch云台电机 6020电机; 6:拨弹电机 2006电机*/
			 CAN_RxHeaderTypeDef rx_header1;
		    uint8_t rx_data1[8];
		
		
static motor_measure_t motor_chassis[7];

static CAN_TxHeaderTypeDef  gimbal_tx_message;
static uint8_t              gimbal_can_send_data[8];
static CAN_TxHeaderTypeDef  chassis_tx_message;
static uint8_t              chassis_can_send_data[8];
static CAN_TxHeaderTypeDef  gimbalcan2_tx_message;
static uint8_t              gimbal_can2_send_data[8];
		
static CAN_TxHeaderTypeDef  Sp_tx_message;
static uint8_t              Sp_can_send_data[8];
		short diff;
		uint8_t Can1_ReceiveBuffer[8];		//can1接收缓冲区
		uint8_t Can2_ReceiveBuffer[8];		//can2接收缓冲区
		
		
		float powerdata[4];
		
/**
  * @brief          hal CAN fifo call back, receive motor data
  * @param[in]      hcan, the point to CAN handle
  * @retval         none
  */
/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
	if(hcan==&hcan1)
	{
	 //CAN_RxHeaderTypeDef rx_header1;
   //uint8_t rx_data1[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header1, rx_data1);

    switch (rx_header1.StdId)
    {
//    case Chassis1_ID:
//    case Chassis2_ID:
//    case Chassis3_ID:
//    case Chassis4_ID:
//        Get_Chassis_Motor_Info(rx_header1.StdId,rx_data1);
//        break;
//    case YAW_ID:
//        Get_Gimbal_Yaw_Motor_Info(rx_header1.StdId,rx_data1);
//        break;
////    case PITCH_ID:
////        Get_Gimbal_Pitch_Motor_Info(rx_header1.StdId,rx_data1);
////        break;
//    case FEED_ID:
//        Get_Feed_Motor_Info(rx_header1.StdId,rx_data1);
//        break;
			case UP_DOWNID:
				Get_UP_Board_Chassis_Info(rx_header1.StdId,rx_data1);
		case 0x211:
					Get_Chaojidianrong(rx_header1.StdId,rx_data1);
    default:
    {
        break;
    }
    }
	}
	if(hcan==&hcan2)
	{
		CAN_RxHeaderTypeDef rx_header2;
    uint8_t rx_data2[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header2, rx_data2);

    switch (rx_header2.StdId)
    {
//    case Fric0_ID:
//    case Fric1_ID:
//		Get_Fric_Motor_Info(rx_header2.StdId,rx_data2);
//    break;
//		case Chassis3_ID:
//		Get_Gimbal_Pitch_Motor_Info(rx_header2.StdId,rx_data2);
	  case Chassis1_ID:
    case Chassis2_ID:
    case Chassis3_ID:
    case Chassis4_ID:
        Get_Chassis_Motor_Info(rx_header2.StdId,rx_data2);
        break;
		case 0x211:
				Get_Chaojidianrong(rx_header2.StdId,rx_data2);
				break;
		default:
			{
        break;
			}
		}
	}
}


void Get_Chaojidianrong(uint32_t motor_id, uint8_t *canbuf_receive)
{
	if(motor_id == 0x211)
	{
		
		powerdata[0]= (canbuf_receive[1]<<8|canbuf_receive[0])/100.0f;//输入电压
		powerdata[1]=(canbuf_receive[2]| canbuf_receive[3]<<8)/100.0f ;//电容电压
		powerdata[2]=(canbuf_receive[4] | canbuf_receive[5]<<8)/100.0f ;//输入电流
		powerdata[3]=(canbuf_receive[6] | canbuf_receive[7]<<8)/100.0f ;//设定功率
	}
}
/**
  * @brief          send control current of motor (0x205, 0x206, 0x207, 0x208)
  * @param[in]      yaw: (0x205) 6020 motor control current, range [-30000,30000] 
  * @param[in]      pitch: (0x206) 6020 motor control current, range [-30000,30000]
  * @param[in]      shoot: (0x207) 2006 motor control current, range [-10000,10000]
  * @param[in]      rev: (0x208) reserve motor control current
  * @retval         none
  */
/**
  * @brief          发送电机控制电流(0x205,0x206,0x207,0x208)
  * @param[in]      yaw: (0x205) 6020电机控制电流, 范围 [-30000,30000]
  * @param[in]      pitch: (0x206) 6020电机控制电流, 范围 [-30000,30000]
  * @param[in]      shoot: (0x207) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      rev: (0x208) 保留，电机控制电流
  * @retval         none
  */
void CAN_cmd_gimbal(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev)
{
    uint32_t send_mail_box;
    gimbal_tx_message.StdId = CAN_GIMBAL_ALL_ID;
    gimbal_tx_message.IDE = CAN_ID_STD;
    gimbal_tx_message.RTR = CAN_RTR_DATA;
    gimbal_tx_message.DLC = 0x08;
    gimbal_can_send_data[0] = (yaw >> 8);
    gimbal_can_send_data[1] = yaw;
    gimbal_can_send_data[2] = (pitch >> 8);
    gimbal_can_send_data[3] = pitch;
    gimbal_can_send_data[4] = (shoot >> 8);
    gimbal_can_send_data[5] = shoot;
    gimbal_can_send_data[6] = (rev >> 8);
    gimbal_can_send_data[7] = rev;
    HAL_CAN_AddTxMessage(/*&GIMBAL_CAN*/&CHASSIS_CAN, &gimbal_tx_message, gimbal_can_send_data, &send_mail_box);
}

/**
  * @brief          send CAN packet of ID 0x700, it will set chassis motor 3508 to quick ID setting
  * @param[in]      none
  * @retval         none
  */
/**
  * @brief          发送ID为0x700的CAN包,它会设置3508电机进入快速设置ID
  * @param[in]      none
  * @retval         none
  */
void CAN_cmd_chassis_reset_ID(void)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x700;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = 0;
    chassis_can_send_data[1] = 0;
    chassis_can_send_data[2] = 0;
    chassis_can_send_data[3] = 0;
    chassis_can_send_data[4] = 0;
    chassis_can_send_data[5] = 0;
    chassis_can_send_data[6] = 0;
    chassis_can_send_data[7] = 0;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}


/**
  * @brief          send control current of motor (0x201, 0x202, 0x203, 0x204)
  * @param[in]      motor1: (0x201) 3508 motor control current, range [-16384,16384] 
  * @param[in]      motor2: (0x202) 3508 motor control current, range [-16384,16384] 
  * @param[in]      motor3: (0x203) 3508 motor control current, range [-16384,16384] 
  * @param[in]      motor4: (0x204) 3508 motor control current, range [-16384,16384] 
  * @retval         none
  */
/**
  * @brief          发送电机控制电流(0x201,0x202,0x203,0x204)
  * @param[in]      motor1: (0x201) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor2: (0x202) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor3: (0x203) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor4: (0x204) 3508电机控制电流, 范围 [-16384,16384]
  * @retval         none
  */
void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = CAN_CHASSIS_ALL_ID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8;
    chassis_can_send_data[1] = motor1;
    chassis_can_send_data[2] = motor2 >> 8;
    chassis_can_send_data[3] = motor2;
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}

/**
  * @brief          发送电机控制电流(0x201,0x202,0x203,0x204)
  * @param[in]      motor1: (0x201) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor2: (0x202) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor3: (0x203) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor4: (0x204) 3508电机控制电流, 范围 [-16384,16384]
  * @retval         none
  */
void CAN2_cmd_(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    gimbalcan2_tx_message.StdId = CAN_CHASSIS_ALL_ID;
    gimbalcan2_tx_message.IDE = CAN_ID_STD;
    gimbalcan2_tx_message.RTR = CAN_RTR_DATA;
    gimbalcan2_tx_message.DLC = 0x08;
    gimbal_can2_send_data[0] = motor1 >> 8;
    gimbal_can2_send_data[1] = motor1;
    gimbal_can2_send_data[2] = motor2 >> 8;
    gimbal_can2_send_data[3] = motor2;
    gimbal_can2_send_data[4] = motor3 >> 8;
    gimbal_can2_send_data[5] = motor3;
    gimbal_can2_send_data[6] = motor4 >> 8;
    gimbal_can2_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&hcan2, &gimbalcan2_tx_message, gimbal_can2_send_data, &send_mail_box);
}



/**
  * @brief          发送超电控制电流(0x201,0x202,0x203,0x204)
  * @param[in]      motor1: (0x201) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor2: (0x202) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor3: (0x203) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor4: (0x204) 3508电机控制电流, 范围 [-16384,16384]
  * @retval         none
  */
void CAN_cmd_SuperPower(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    Sp_tx_message.StdId = 0x210;
    Sp_tx_message.IDE = CAN_ID_STD;
    Sp_tx_message.RTR = CAN_RTR_DATA;
    Sp_tx_message.DLC = 0x08;
    Sp_can_send_data[0] = motor1 >> 8;
    Sp_can_send_data[1] = motor1;
    Sp_can_send_data[2] = motor2 >> 8;
    Sp_can_send_data[3] = motor2;
    Sp_can_send_data[4] = motor3 >> 8;
    Sp_can_send_data[5] = motor3;
    Sp_can_send_data[6] = motor4 >> 8;
    Sp_can_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &Sp_tx_message, Sp_can_send_data, &send_mail_box);
}

/**
  * @brief          return the yaw 6020 motor data point
  * @param[in]      none
  * @retval         motor data point
  */
void CAN_cmd_DOWNtoUP(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = DOWN_UPID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8;
    chassis_can_send_data[1] = motor1;
    chassis_can_send_data[2] = motor2 >> 8;
    chassis_can_send_data[3] = motor2;
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}

void CAN_cmd_referee(uint8_t color, int8_t BltSpd,uint16_t powerlimit)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = DOWN_UPID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = 0x77;
    chassis_can_send_data[1] = color;
    chassis_can_send_data[2] = 0x76;
    chassis_can_send_data[3] = BltSpd;
    chassis_can_send_data[4] = powerlimit>>8;
    chassis_can_send_data[5] = powerlimit;
    chassis_can_send_data[6] = 0;
    chassis_can_send_data[7] = 0;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}

/**
  * @brief          return the yaw 6020 motor data point
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          返回yaw 6020电机数据指针
  * @param[in]      none
  * @retval         电机数据指针
  */
const motor_measure_t *get_yaw_gimbal_motor_measure_point(void)
{
    return &motor_chassis[4];
}

/**
  * @brief          return the pitch 6020 motor data point
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          返回pitch 6020电机数据指针
  * @param[in]      none
  * @retval         电机数据指针
  */
const motor_measure_t *get_pitch_gimbal_motor_measure_point(void)
{
    return &motor_chassis[5];
}


/**
  * @brief          return the trigger 2006 motor data point
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          返回拨弹电机 2006电机数据指针
  * @param[in]      none
  * @retval         电机数据指针
  */
const motor_measure_t *get_trigger_motor_measure_point(void)
{
    return &motor_chassis[6];
}


/**
  * @brief          return the chassis 3508 motor data point
  * @param[in]      i: motor number,range [0,3]
  * @retval         motor data point
  */
/**
  * @brief          返回底盘电机 3508电机数据指针
  * @param[in]      i: 电机编号,范围[0,3]
  * @retval         电机数据指针
  */
const motor_measure_t *get_chassis_motor_measure_point(uint8_t i)
{
    return &motor_chassis[(i & 0x03)];
}
