#include "ramp.h"
#include "tim.h"

volatile uint32_t ramp_cnt = 0;		//在定时器中自增，为斜坡函数提供支持

//斜坡函数使用方法请参照chassis_ctrl.c中键盘控制部分


//斜坡开始
void Ramp_Set( Ramp *ramp)
{
	ramp->time = ramp_cnt;
}

float f_Ramp_Calc(float input,float target,float ramp)
{
  float error = target - input;
  float output = input;

	if (error > 0){
        if (error > ramp){output += ramp;}   
        else{output += error;}
    }else{
        if (error < -ramp){output += -ramp;}
        else{output += error;}
    }

    return output;
}

//斜坡函数计算
float Ramp_Calc( Ramp *ramp, uint32_t time, uint8_t dir)
{
	uint32_t dif;
	
	dif = ramp_cnt - ramp->time;
	ramp->time = ramp_cnt;
	
	switch( dir)
	{
		case RAMP_UP:
			ramp->ramp_out += ( float)dif * 2.2f / ( float)time;
		break;
		case RAMP_DOWN:
			ramp->ramp_out -= ( float)dif * 2.2f / ( float)time;
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


float LPFfc = 0.5f;     //截止频率
float alpha = 0;     //滤波系数

/************************ 滤波器初始化 alpha *****************************/
void low_pass_filter_init(void)
{
  float bb = 2.0 * 3.14159f * LPFfc * 0.02f;
  alpha = bb / (bb + 1);
}

float low_pass_filter(float value)
{
  static float out_last = 0; //上一次滤波值
  float out;

  /***************** 如果第一次进入，则给 out_last 赋值 ******************/
  static char fisrt_flag = 1;
  if (fisrt_flag == 1)
  {
    fisrt_flag = 0;
    out_last = value;
  }

  /*************************** 一阶滤波 *********************************/
  out = out_last + alpha * (value - out_last);
  out_last = out;

  return out;
}
