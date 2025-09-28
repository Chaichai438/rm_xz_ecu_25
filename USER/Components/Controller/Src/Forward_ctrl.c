#include "Forward_ctrl.h"
#include "main.h"

#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }


/**
  * @brief          Forward_Ctrl_init
  * @param[in]      FDC: 前馈结构数据指针
  * @param[in]      mode: FORWARD_CTRL_MODE_D1:一阶差分前馈
  *                 FORWARD_CTRL_MODE_D2: 二阶差分前馈
  * @param[in]      Kf：[0]一阶系数，[1]二阶系数
  * @param[in]      max_out: 前馈最大输出
  * @retval         none
  */
void Forward_Ctrl_init(FORWARD_CTRL_T* FDC, char mode, const float Kf[2], float max_out)
{
    if (FDC == NULL || FDC == NULL)
    {
        return;
    }
    FDC->mode         =   mode;
    FDC->k1f          =   Kf[0];
    FDC->k2f          =   Kf[1];
    FDC->max_out      =   max_out;
    FDC->Target[0]    =   FDC->Target[1]    = 0.0f;
    FDC->Delta_1_T[0] =   FDC->Delta_1_T[1] = 0.0f;
    FDC->Delta_2_T    =   0.0f;
    FDC->Out          =   0.0f;
}

/**
  * @brief          前馈计算
  * @param[out]     FDC: FDC结构数据指针
  * @param[in]      set: 设定值
  * @retval         前馈输出
  */
float FDC_calc(FORWARD_CTRL_T *FDC,float set)
{
    if (FDC == NULL)
    {
        return 0.0f;
    }
    FDC->Target[1] = FDC->Target[0];
    FDC->Target[0] = set;
    FDC->Delta_1_T[1] = FDC->Delta_1_T[0];
    FDC->Delta_1_T[0] = FDC->Target[0] - FDC->Target[1];
    if (FDC->mode == FORWARD_CTRL_MODE_D1)
    {
      FDC->Out = FDC->k1f * FDC->Delta_1_T[0];
    }
    else if (FDC->mode == FORWARD_CTRL_MODE_D2)
    {
      FDC->Delta_2_T = FDC->Delta_1_T[0] - FDC->Delta_1_T[1];
      FDC->Out = FDC->k1f * FDC->Delta_1_T[0] + FDC->k2f * FDC->Delta_2_T;
    }
    LimitMax(FDC->Out, FDC->max_out);
    return FDC->Out;
}
