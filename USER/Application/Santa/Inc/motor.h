#ifndef __MOTOR_H
#define __MOTOR_H
#include "struct_typedef.h"

#include <stdint.h>

typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_3508_M1_ID = 0x201,
    CAN_3508_M2_ID = 0x202,
    CAN_3508_M3_ID = 0x203,
    CAN_3508_M4_ID = 0x204,

    CAN_YAW_MOTOR_ID = 0x205,
    CAN_PIT_MOTOR_ID = 0x206,
    CAN_TRIGGER_MOTOR_ID = 0x207,
    CAN_GIMBAL_ALL_ID = 0x1FF,

} can_msg_id_e;

typedef enum
{
    CAN_CHASSIS_ALL_ID2 = 0x200,
    CAN_3508_M1_ID2 = 0x201,
    CAN_3508_M2_ID2 = 0x202,
} can2_msg_id_e;

void CAN1_Send_Msg( uint32_t stdid, u8 *msg, u8 len);
void CAN2_Send_Msg( uint32_t stdid, u8 *msg, u8 len);
 //void    Get_Chaojidianrong();
#endif

