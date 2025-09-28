#include "ramp.h"
#include "tim.h"

volatile u32 ramp_cnt = 0;		//在定时器中自增，为斜坡函数提供支持

//斜坡函数使用方法请参照chassis_ctrl.c中键盘控制部分


//斜坡开始
void Ramp_Set( Ramp *ramp)
{
	ramp->time = ramp_cnt;
}



//斜坡函数计算
float Ramp_Calc( Ramp *ramp, u32 time, u8 dir)
{
	u32 dif;
	
	dif = ramp_cnt - ramp->time;
	ramp->time = ramp_cnt;
	
	switch( dir)
	{
		case RAMP_UP:
//			if(ramp_cnt>0.1)
//			ramp->ramp_out += ( float)dif *3.8f / ( float)time;
//			else
				ramp->ramp_out += ( float)dif * 2.0f / ( float)time;
		break;
		case RAMP_DOWN:
//			if(ramp_cnt>0.1)
//			ramp->ramp_out -= ( float)dif * 3.8f / ( float)time;
//			else
			ramp->ramp_out -= ( float)dif * 2.0f / ( float)time;
			
		break;
	}
	
//	printf("dif/time=%f\r\n",( float)dif / ( float)time);
	
	if( ramp->ramp_out > 1.0f)
	{
		ramp->ramp_out = 1.0f;
		return 1.0f;	
	}
	if( ramp->ramp_out < 0.0f)
	{
		ramp->ramp_out = 0.0f;
		return 0.0f;
	}
		
	return ramp->ramp_out;
	
//	if( dif * 5 > time)
//		return 1.0f;
//	
//	return dif * 5.0f / time;
}





