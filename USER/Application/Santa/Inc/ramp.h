#ifndef RAMP_H_INCLUDED
#define RAMP_H_INCLUDED

#include "struct_typedef.h"


#define RAMP_UP		1			//增
#define RAMP_DOWN	2			//减

typedef struct
{
	u32 time;
	float ramp_out;
} Ramp;


void Ramp_Set( Ramp *ramp);								//斜坡开始
float Ramp_Calc( Ramp *ramp, u32 time, u8 dir);			//斜坡计算，time单位为ms			

extern volatile u32 ramp_cnt;




#endif

