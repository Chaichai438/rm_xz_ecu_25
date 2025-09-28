#ifndef BSP_FRIC_H
#define BSP_FRIC_H
#include "struct_typedef.h"
#include "gimbal_task.h"
#include "user_lib.h"
#define FRIC_OFF 1050
#define FRIC_UP 1510
#define FRIC_DOWN 100


#define SHOOT_CONTROL_TIME          GIMBAL_CONTROL_TIME
#define SHOOT_FRIC_PWM_ADD_VALUE    120.0f
#define SHOOT_FRIC_PWM_CLOSE 100.0f

extern ramp_function_source_t fric1_ramp,fric2_ramp;        //�������
extern uint16_t fric_pwm1,fric_pwm2;

extern void fric_off(void);
extern void fric1_on(uint16_t cmd);
extern void fric2_on(uint16_t cmd);
extern void fric_init(void);
void Fric_on(void);
void Fric_off(void);
#endif
