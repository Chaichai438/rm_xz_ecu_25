#include "gyro.h"
#include "BMI088.h"



//uint32_t YawKeyNum=0;
unsigned int Yaw_ADDNUM;
float GYRO_YAW_ANGLE_C_X=0,GYRO_YAW_ANGLE_C_LAST_X=0;
float GYRO_PITCH_ANGLE_C_X=0,GYRO_PITCH_ANGLE_C_LAST_X=0;
float GYRO_YAW_XHH,GYRO_PITCH_XHH;
volatile float GYRO_YAW_ANGLE_GET_X,GYRO_PITCH_ANGLE_GET_X,GYRO_G_Z_XHH,GYRO_G_X_XHH;
float GYRO_diff_x=0,GYRO_diff_y;
int GYRO_n_xhh=0;
float Pitch_G_X_XHH[3];
float Pitch_fliternum[2]={0,0};
extern fp32 INS_angle[3];      //euler angle, unit rad.欧拉角 单位 rad
/*****kalman*******/
KFP KFP_Pitch_G={0.02,0,0,0,0.001,0.543};
float kalman_Pitch_G=0;
/*************/
fp32 GYRO_euler_angle_fp32[3];
fp32 GYRO_gyro_fp32[3];

float del=0;

void gyro_get(void)
{
	GYRO_YAW_XHH=(GYRO_euler_angle_fp32[0]/(3.1415926*2))*360.0;
	GYRO_PITCH_XHH=(GYRO_euler_angle_fp32[1]/(3.1415926*2))*360.0;
	GYRO_G_Z_XHH=GYRO_gyro_fp32[2]*100;
	GYRO_G_X_XHH=GYRO_gyro_fp32[1]*100;	
//	Pitch_G_X_XHH[0] = Pitch_G_X_XHH[1];
//	Pitch_G_X_XHH[1] = Pitch_G_X_XHH[2];
//	Pitch_G_X_XHH[2] = GYRO_G_X_XHH;
	del=YAW_XHH-GYRO_YAW_XHH;
	kalman_Pitch_G = kalmanFilter(&KFP_Pitch_G,GYRO_G_X_XHH);

	GYRO_YAW_ANGLE_C_X=GYRO_YAW_XHH;
	if(GYRO_n_xhh>400){
		GYRO_diff_x=GYRO_YAW_ANGLE_C_X-GYRO_YAW_ANGLE_C_LAST_X;
	}
	if(GYRO_diff_x>=300)  
	{	
		GYRO_diff_x=GYRO_diff_x-360;
	}
	else if (GYRO_diff_x<=-300)
	{
		GYRO_diff_x=GYRO_diff_x+360;
	}
	GYRO_YAW_ANGLE_C_LAST_X=GYRO_YAW_ANGLE_C_X;
	GYRO_YAW_ANGLE_GET_X+=GYRO_diff_x;

	GYRO_PITCH_ANGLE_C_X=GYRO_PITCH_XHH;		
	if(GYRO_n_xhh>400){
		GYRO_diff_y=GYRO_PITCH_ANGLE_C_X-GYRO_PITCH_ANGLE_C_LAST_X;
	}
	if(GYRO_diff_y>=300)  {	
		GYRO_diff_y=GYRO_diff_y-360;
	}
	else if (GYRO_diff_y<=-300){
		GYRO_diff_y=GYRO_diff_y+360;
	}
	GYRO_PITCH_ANGLE_C_LAST_X=GYRO_PITCH_ANGLE_C_X;
	GYRO_PITCH_ANGLE_GET_X+=GYRO_diff_y;



	if(GYRO_n_xhh<500)GYRO_n_xhh++;
		
}

//2. 以高度为例 定义卡尔曼结构体并初始化参数


/**
 *卡尔曼滤波器
 *@param KFP *kfp 卡尔曼结构体参数
 *   float input 需要滤波的参数的测量值（即传感器的采集值）
 *@return 滤波后的参数（最优值）
 */
 float kalmanFilter(KFP *kfp,float input)
 {
     //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
     kfp->Now_P = kfp->LastP + kfp->Q;
     //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
     kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
     //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
     kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//因为这一次的预测值就是上一次的输出值
     //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
     kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
     return kfp->out;
 }

/**
 *调用卡尔曼滤波器 实践
 */

