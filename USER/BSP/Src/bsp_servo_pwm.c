#include "bsp_servo_pwm.h"
#include "main.h"
#include "remote_control.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim8;
extern RC_ctrl_t rc_ctrl;
void servo_pwm_set(uint16_t pwm, uint8_t i)
{
    switch(i)
    {
        case 0:
        {
            __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, pwm);
        }break;
        case 1:
        {
            __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, pwm);
        }break;
        case 2:
        {
				__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, pwm);
        }break;
        case 3:
        {     
					  
							if(RC_Key_Read( RC_KEY_R) == 1||rc_ctrl.rc.ch[4] >=550)
								__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1,1920);
							else
								__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1,850);
					
            
                
			}break;
    }
}
