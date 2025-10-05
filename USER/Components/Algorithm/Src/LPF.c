/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : LPF.c
  * @brief          : lowpass filter 
  * @author         : GrassFan Wang
  * @date           : 2025/12/28
  * @version        : v1.0
  ******************************************************************************
  * @attention      : To be perfected
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "LPF.h"


float sign(float input){
   return (input>0.0f) - (input<0.0f);

}

/**
  * @brief ��ʼ��һ�׵�ͨ�˲���.
  * @param LPF: һ�׵�ͨ�˲����ṹ��.
  * @param Alpha: �˲���ϵ��.
  * @param Frame_Period: ��������.
  * @retval ��.
  */
void LowPassFilter1p_Init(LowPassFilter1p_Info_TypeDef *LPF,float Alpha)
{
  LPF->Alpha = Alpha;
  LPF->Input = 0;
  LPF->Output = 0;
}

/**
  * @brief ����һ�׵�ͨ�˲�������.
  * @param Input: ��ǰ����.
  * @retval �˲����ֵ.
  */
float LowPassFilter1p_Update(LowPassFilter1p_Info_TypeDef *LPF,float Input)
{
  LPF->Input = Input;

  if(LPF->Initialized == false)
  {
    LPF->Output = LPF->Input;//��һ�ν�����º��� ��������������
    LPF->Initialized = true;
  }

	/*�˲���ϵ�� = Alhpa 0< Alhpa <1 ���˲�ϵ��ԽС���˲�����Խƽ�ȣ������ͺ��Ը���
                                   ���˲�ϵ��Խ���˲�����Խ�ӽ�ʵ��ֵ���ͺ���С�������˲����߸��� */
	
	//�˲�ֵ = Alhpa * ��һ����� + ��1 - Alhpa)*�������	
  LPF->Output = LPF->Alpha * LPF->Output +(1.f - LPF->Alpha) * LPF->Input;
	
  return LPF->Output;
}

/**
  * @brief ��ʼ�����׵�ͨ�˲���.
  * @param Alpha[3]: �˲���ϵ��[3].
  * @retval ��.
  */
void LowPassFilter2p_Init(LowPassFilter2p_Info_TypeDef *LPF,float Alpha[3])
{
  memcpy(LPF->Alpha,Alpha,sizeof(LPF->Alpha));
  LPF->Input = 0;
  memset(LPF->Output,0,sizeof(LPF->Output));
}

/**
  * @brief ���¶��׵�ͨ�˲�������.
  * @param Input: ��ǰ����.
  * @retval �˲����ֵ.
  */
float LowPassFilter2p_Update(LowPassFilter2p_Info_TypeDef *LPF,float Input)
{
	LPF->Input = Input;
  
  if(LPF->Initialized == false)
  {
    LPF->Output[0] = LPF->Input;
    LPF->Output[1] = LPF->Input;
    LPF->Output[2] = LPF->Input;
    LPF->Initialized = true;
  }
  
	LPF->Output[0] = LPF->Output[1];
	LPF->Output[1] = LPF->Output[2];
  LPF->Output[2] = LPF->Alpha[0] * LPF->Output[1] + LPF->Alpha[1] * LPF->Output[0] + LPF->Alpha[2] * LPF->Input;

	return LPF->Output[2];
}


/**
 * 计算低通滤波器的权重因子
 * @param tau 时间常数，决定滤波器的响应速度
 * @param dt 时间步长，表示每次更新的时间间隔
 * @return 返回计算出的权重因子alpha
 */
static float compute_alpha(float tau, float dt) {
    // 强制非法参数返回最大权重
    if (tau < 0.0f || dt <= 0.0f) return 1.0f;
    // 计算权重因子，防止超过界限
    float a = dt / (tau + dt);
    if (a > 1.0f) a = 1.0f;
    if (a < 0.0f) a = 0.0f;
    return a;
}

/**
 * 初始化低通滤波器
 * @param filter 滤波器对象指针
 * @param tau 时间常数
 * @param dt 时间步长
 * @param initial_output 初始输出值
 */
void LPF_Init(LowPassFilter* filter, float tau, float dt, float initial_output) {
    filter->tau = tau;
    filter->dt = dt;
    filter->output = initial_output;
    filter->enabled = 1;  // 默认启用
    filter->alpha = compute_alpha(tau, dt);
}


/**
 * 更新低通滤波器的输出
 * @param filter 滤波器对象指针
 * @param input 输入值
 * @return 返回更新后的输出值
 */
float LPF_Update(LowPassFilter* filter, float input) {
    if (!filter->enabled) {
        filter->output = input;
        return input;
    }

    // 防止输入是非法值
    if (isnan(input) || isinf(input)) {
        return filter->output;
    }

    float alpha = filter->alpha;

    // 防止 alpha 异常
    if (alpha < 0.0f || alpha > 1.0f) {
        alpha = 1.0f;
    }

    // 核心滤波计算
    filter->output = (1.0f - alpha) * filter->output + alpha * input;

    // 防止输出溢出
    if (isinf(filter->output) || isnan(filter->output)) {
        filter->output = 0.0f;
    }

    return filter->output;
}

//------------------------------------------------------------------------------


