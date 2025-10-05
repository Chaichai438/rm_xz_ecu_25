#include "stm32h723xx.h"
#include "SMC_EGA.h"
#include "math.h"

int8_t Signal(float y)
{
	if (y > 0)
		return 1;
	else if (y == 0)
		return 0;
	else
		return -1;
}

	// 饱和函数
float Sat(float y)
{
	if (fabs(y) <= 1)
		return y;
	else
		return Signal(y);
}
	// 符号函数,若有抖动可以换个陡峭的饱和函数


void SMC_Para_Init(SMC_t *SMC_Struct,float C,float K,float ref,float error_eps,float u_max,float J,float epsilon)
{
	SMC_Struct->C 				= 			C;
	SMC_Struct->K 				= 			K;
	SMC_Struct->ref 			= 			ref;
	SMC_Struct->error_eps = 			error_eps;
	SMC_Struct->u_max 		=				u_max;
	SMC_Struct->J 				=				J;
	SMC_Struct->epsilon 	= 			epsilon;
}
	
	
void SMC_Tick(float angle_now,float angle_vel,SMC_t *SMC_Struct)
{
    SMC_Struct->angle = angle_now;
    SMC_Struct->ang_vel = angle_vel;
	SMC_Struct->error = SMC_Struct->angle - SMC_Struct->ref;
	SMC_Struct->ddref = (SMC_Struct->ref - SMC_Struct->refl) - SMC_Struct->dref; //这里对前馈进行了处理，没有严格单位统一
	SMC_Struct->dref = (SMC_Struct->ref - SMC_Struct->refl);
	//误差下限处理
	if (fabs(SMC_Struct->error) < SMC_Struct->error_eps)
	{
		SMC_Struct->u = 0;
		return;
	}
	//smc surface
	SMC_Struct->s = SMC_Struct->C * SMC_Struct->error + (SMC_Struct->ang_vel - SMC_Struct->dref);
	SMC_Struct->u = SMC_Struct->J * (SMC_Struct->ddref - SMC_Struct->C * (SMC_Struct->ang_vel - SMC_Struct->dref) - SMC_Struct->epsilon * Sat(SMC_Struct->s) - SMC_Struct->K * SMC_Struct->s);
	//控制量限幅
	if (SMC_Struct->u > SMC_Struct->u_max)
		SMC_Struct->u = SMC_Struct->u_max;
	if (SMC_Struct->u < -SMC_Struct->u_max)
		SMC_Struct->u = -SMC_Struct->u_max;
	//参数更新
	SMC_Struct->refl = SMC_Struct->ref;

}