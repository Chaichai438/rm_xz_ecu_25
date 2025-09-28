#ifndef CHASSIS_POWER_H_INCLUDED
#define CHASSIS_POWER_H_INCLUDED

#include "struct_typedef.h"
#include <stdint.h>


#define WD 30   

#define T_POWER 0.02


float chassis_pmax(void);
void chassis_power(short *motor_current);

void Get_Power_Info(uint32_t id, u8 *canbuf_receive);

//void test (void);

#endif 
