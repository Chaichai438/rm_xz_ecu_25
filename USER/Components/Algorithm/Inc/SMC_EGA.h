#ifndef _SMC_EGA_H_
#define _SMC_EGA_H_

#define SMC_COMMON_PARAM 20,120,0,0.001,25000,0.8,0.5
#define SMC_Yaw_PARAM 13,100,0,0.001,25000,0.8,0.5

typedef struct{

    float C;
	float K;
	float ref; //初始目标值
	float error_eps;//误差下限
	float u_max;//输出最大值
	float J;//估计惯量
	float angle; //角度反馈，°
	float ang_vel;//角速度反馈，°/s
	float epsilon;

    float u;

	float error;
	float error_last;
	float dref;//目标值一阶导
	float ddref;//目标值二阶导
	float refl;//上一次的目标值

	float s;//滑模面
}SMC_t;



void SMC_Para_Init(SMC_t *SMC_Struct,float C,float K,float ref,float error_eps,float u_max,float J,float epsilon);
void SMC_Tick(float angle_now,float angle_vel,SMC_t *SMC_Struct);
#endif