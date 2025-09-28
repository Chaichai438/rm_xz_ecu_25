#include "init.h"
#include "bsp_delay.h"
#include "remote_control.h"
#include "bsp_usart.h"
#include "chassis_ctrl.h"
#include "gimbal_ctrl.h"
#include "ammunition_feed_ctrl.h"
#include "bsp_can.h"
#include "shoot_fric.h"

void RM_init(void)
{
		can_filter_init();
    delay_init();
//    cali_param_init();
//    remote_control_init();
		usart1_Init();
		Chassis_Init();
		Gimbal_Init();
		Ammunition_Init();
		Shoot_fric_pid_init();
}



