/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : lowpass_filter.c
  * @brief          : lowpass filter 
  * @author         : Yan Yuanbin
  * @date           : 2023/04/27
  * @version        : v1.0
  ******************************************************************************
  * @attention      : To be perfected
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LOWPASS_FILTER_H
#define LOWPASS_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "config.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief  һ�׵�ͨ�˲�����Ϣ�Ľṹ��
 */
typedef struct
{
	bool Initialized;       //��ʼ����־ 0:δ��ʼ�� 1����ʼ��
    float Input;          //��ǰ����
    float Output;         //���
    float Alpha;          //�˲�ϵ��
}LowPassFilter1p_Info_TypeDef;

/**
 * @brief ���׵�ͨ�˲�����Ϣ�Ľṹ��.
 */
typedef struct {
    float tau;       // 时间常数（控制平滑程度）
    float dt;        // 采样周期（单位：秒）
    float alpha;     // 滤波系数
    float output;    // 当前输出
    int enabled;     // 是否启用滤波：1=启用，0=绕过
} LowPassFilter;


typedef struct 
{
    bool Initialized;  //��ʼ����־ 0:δ��ʼ�� 1����ʼ��
    float Input;       //��ǰ����
    float Output[3];   //��� ���ں͹�ȥ���ε����
    float Alpha[3];    //�����˲���ϵ��

}LowPassFilter2p_Info_TypeDef;


/* Extern Functions Prototypes ---------------------------------------------*/

/**
  * @brief ���ݺ�����ָ���Ĳ�����ʼ��һ�׵�ͨ�˲���.
  */
extern void LowPassFilter1p_Init(LowPassFilter1p_Info_TypeDef *LPF,float Alpha);
/**
  * @brief ���ݺ�����ָ���Ĳ������£����㣩һ�׵�ͨ�˲���
  */
extern float LowPassFilter1p_Update(LowPassFilter1p_Info_TypeDef *lpf,float input);
/**
  * @brief ���ݺ�����ָ���Ĳ�����ʼ�����׵�ͨ�˲���
  */
extern void  LowPassFilter2p_Init(LowPassFilter2p_Info_TypeDef *lpf,float alpha[3]);
/**
  * @brief ���ݺ�����ָ���Ĳ������£����㣩���׵�ͨ�˲���
  */
extern float LowPassFilter2p_Update(LowPassFilter2p_Info_TypeDef *lpf,float input);
extern float LPF_Update(LowPassFilter* filter, float input);
extern void LPF_Init(LowPassFilter* filter, float tau, float dt, float initial_output);
extern float sign(float input);
#endif //LOWPASS_FILTER_H
