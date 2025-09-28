#ifndef FORWARD_CTRL_H_
#define FORWARD_CTRL_H_

#include "pid.h"


#define FORWARD_CTRL_MODE_D1 1
#define FORWARD_CTRL_MODE_D2 2

typedef struct {
    char mode;
    float k1f;
    float k2f;

    float Target[2];

    float Delta_1_T[2];
    float Delta_2_T;
    float Out;
    float max_out;
}FORWARD_CTRL_T;

typedef struct {
  pid_type_def pid;
    FORWARD_CTRL_T FDC;
}PID_FDC_T;

/**
  * @brief          Forward_Ctrl_init
  * @param[in]      FDC: 前馈结构数据指针
  * @param[in]      mode: FORWARD_CTRL_MODE_D1:一阶差分前馈
  *                 FORWARD_CTRL_MODE_D2: 二阶差分前馈
  * @param[in]      Kf：[0]一阶系数，[1]二阶系数
  * @param[in]      max_out: 前馈最大输出
  * @retval         none
  */
void Forward_Ctrl_init(FORWARD_CTRL_T* FDC, char mode, const float Kf[2], float max_out);

/**
  * @brief          前馈计算
  * @param[out]     FDC: FDC结构数据指针
  * @param[in]      set: 设定值
  * @retval         前馈输出
  */
float FDC_calc(FORWARD_CTRL_T *FDC,float set);



#endif 
