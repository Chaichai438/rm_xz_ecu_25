#include "bsp_fric.h"
#include "main.h"
#include "user_lib.h"
#include "gimbal_task.h"
#include "referee_st.h"

uint16_t fric_pwm1,fric_pwm2;
extern TIM_HandleTypeDef htim1;
ramp_function_source_t fric1_ramp,fric2_ramp;        //Éä»÷Êý¾Ý
extern Game_info game_info;
void fric_off(void)
{
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, FRIC_OFF);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, FRIC_OFF);
}
void fric1_on(uint16_t cmd)
{
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, cmd);
}
void fric2_on(uint16_t cmd)
{
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, cmd);
}

void fric_init(void)
{

    ramp_init(&fric1_ramp, SHOOT_CONTROL_TIME * 0.001f, FRIC_UP, FRIC_OFF);
    ramp_init(&fric2_ramp, SHOOT_CONTROL_TIME * 0.001f, FRIC_UP, FRIC_OFF);

		fric_pwm1 = FRIC_OFF;
    fric_pwm2 = FRIC_OFF;
}



