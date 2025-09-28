#ifndef AMMUNITION_FEED_H_INCLUDED
#define AMMUNITION_FEED_H_INCLUDED

#include "struct_typedef.h"
#include "pid_st.h"
#include <stdint.h>

typedef struct
{
	u16 real_angle;						//ʵ�ʽǶȣ�һȦ8192
	signed short rpm;
	signed short real_current;
	u8 temperature;
	
	
	u16 last_real_angle;				//�ϴ�ʵ�ʽǶ�
	int angle;							//�����Ƕ�ֵ
	
	int last_angle;
	
	int angle_set;						//�趨�Ƕȣ�һȦ8192
	
} ammunition_motor_back;




void Ammunition_pid_init(void);
void Ammunition_Motor_Speed_Set( short motor_speed);
void Get_Ammunition_Motor_Info( uint32_t motor_id, u8 *canbuf_receive);




extern pid_t ammunition_spd_pid;
extern pid_t ammunition_pos_pid;
extern ammunition_motor_back ammunition_motor_info;



#endif // AMMUNITION_FEED_H
