#ifndef  __CHASSIS_H
#define  __CHASSIS_H

#define CHASSIS_VX_MAX 3.0f
#define CHASSIS_A 0.3f
#define CHASSIS_B 0.3f 
#define CHASSIS_VYAW_MAX 1.5f
#define CHASSIS_FOLLOW_ANGLE_ALLOWANCE	10		//µ×ÅÌ¸úËæ½Ç¶ÈÓàÁ¿
void Chassis_Ctrl(void);
void chassis_3508_spd(void);
void chassis_init(void);
float Chassis_Follow( void);

#endif

