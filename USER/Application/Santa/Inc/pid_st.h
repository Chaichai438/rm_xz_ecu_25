#ifndef __PID_ST_H
#define __PID_ST_H

#include "struct_typedef.h"
#include <stdint.h>
enum
{
	LLAST = 0,
	LAST = 1,
	NOW = 2,
	
	POSITION_PID,		//λ��ʽpid
	DELTA_PID			//����ʽpid
};


typedef struct
{
		float p;
    float i;
    float d;
    float last_p;
		float last_i;
		float last_d;
	
    float set[3];						//Ŀ��ֵ,����NOW�� LAST�� LLAST���ϴ�
    float get[3];						//����ֵ
    float err[3];						//���

    
    float pout;							//p���
    float iout;							//i���
    float dout;							//d���
    
    float pos_out;						//����λ��ʽ���
    float last_pos_out;					//�ϴ����
    float delta_u;						//��������ֵ
    float delta_out;					//��������ʽ��� = last_delta_out + delta_u
    float last_delta_out;
    
	float max_err;
	float deadband;						//err < deadband return
	
    uint32_t pid_mode;
    uint32_t MaxOutput;					//����޷�
    uint32_t IntegralLimit;				//�����޷�
    
} pid_t;



void pid_init( pid_t *pid, uint32_t mode, uint32_t max_out, uint32_t integral_limit, float kp, float ki, float kd);					//PID��ʼ��
float pid_calc( pid_t *pid, float get, float set);																					//����PID
void pid_reset( pid_t *pid, float kp, float ki, float kd);																			//����PID





#endif // PID_H_INCLUDED
