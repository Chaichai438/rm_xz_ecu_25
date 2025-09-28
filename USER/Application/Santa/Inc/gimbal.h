#ifndef  __GIMBAL_H
#define  __GIMBAL_H
#include "struct_typedef.h"
#include <stdint.h>
//#define GIMBAL_YAW_OFFSET  5600
#define GIMBAL_PITCH_OFFSET  6844

void Get_Gimbal_Motor_Info( uint32_t motor_id, uint8_t *canbuf_receive);
void Gimbal_Control_Func(void);
void gimbal_PID_Init(void);
#endif

