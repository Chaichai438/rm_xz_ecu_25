#include "powerlimit.h"
#include "referee_st.h"
#include "CAN_receive.h"

/*********外部引用***********/
extern ext_robot_state_t             game_robot_state;                    // 0x0201  比赛机器人状态
extern Game_info game_info;

/*********全局变量***********/

float Power,Power_Buffer;
uint16_t Chassis_powerlim2_Sp;

/*********函数定义***********/

void chaojidianrong_can(void)
{
	uint8_t send_can[2];
	Chassis_powerlim2_Sp=(game_info.game_robot_state.chassis_power_limit-5.0f )* 100.0f;
	
	
	send_can[0]=Chassis_powerlim2_Sp >> 8;
	send_can[1]=Chassis_powerlim2_Sp;
	CAN_cmd_SuperPower(send_can[0],send_can[1],0,0);
}

