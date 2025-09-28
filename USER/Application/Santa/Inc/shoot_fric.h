#ifndef SHOOT_FRIC_H
#define SHOOT_FRIC_H
#include "struct_typedef.h"
#include "pid_st.h"

 void Shoot_fric_pid_init(void);
 void Get_Shoot_Motor_Info( uint32_t motor_id, u8 *canbuf_receive);
 void Shoot_Speed_Set(int shoot_spd);
 void Shoot_fric_Ctrl(void);
#endif
