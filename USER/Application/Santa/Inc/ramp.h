#ifndef RAMP_H_INCLUDED
#define RAMP_H_INCLUDED

#include "sys.h"
#include "struct_typedef.h"
#include <stdint.h>
#define RAMP_UP		1			//��
#define RAMP_DOWN	2			//��

typedef struct
{
	uint32_t time;
	float ramp_out;
} Ramp;

void low_pass_filter_init(void);
float low_pass_filter(float value);
void Ramp_Set( Ramp *ramp);								//б�¿�ʼ
float Ramp_Calc( Ramp *ramp, uint32_t time, uint8_t dir);			//б�¼��㣬time��λΪms			
float f_Ramp_Calc(float input,float target,float ramp);
extern volatile uint32_t ramp_cnt;




#endif

