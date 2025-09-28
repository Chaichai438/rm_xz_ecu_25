#include "referee_st.h"
#include "string.h"
#include "stdio.h"
#include "CRC8_CRC16.h"
#include "protocol.h"

Game_info game_info;						//裁判系统数据
frame_header_structUI_t referee_receive_header;
frame_header_structUI_t referee_send_header;

void init_referee_struct_data(void)
{
	memset(&game_info, 0, sizeof(Game_info)); 		//初始化结构体
    memset(&referee_receive_header, 0, sizeof(frame_header_structUI_t));   
    memset(&referee_send_header, 0, sizeof(frame_header_structUI_t));

    memset(&game_info.game_state, 0, sizeof(ext_game_state_t));
    memset(&game_info.game_result, 0, sizeof(ext_game_result_t));
    memset(&game_info.game_robot_HP_t, 0, sizeof(ext_game_robot_HP_t));
	memset(&game_info.dart_status, 0, sizeof(ext_dart_status_t));
	memset(&game_info.ICRA_buff, 0, sizeof(ext_ICRA_buff_debuff_zone_status_t));
	
	memset(&game_info.event_data, 0, sizeof(ext_event_data_t));
    memset(&game_info.supply_projectile_action, 0, sizeof(ext_supply_projectile_action_t));
    memset(&game_info.referee_warning_t, 0, sizeof(ext_referee_warning_t));
	memset(&game_info.dart_remaining_time_t, 0, sizeof(ext_dart_remaining_time_t));


    memset(&game_info.game_robot_state, 0, sizeof(ext_game_robot_state_t));
    memset(&game_info.power_heat_data, 0, sizeof(ext_power_heat_data_t));
    memset(&game_info.game_robot_pos, 0, sizeof(ext_game_robot_pos_t));
    memset(&game_info.buff_musk, 0, sizeof(ext_buff_t));
    memset(&game_info.aerial_robot_energy, 0, sizeof(aerial_robot_energy_t));
    memset(&game_info.robot_hurt, 0, sizeof(ext_robot_hurt_t));
    memset(&game_info.shoot_data, 0, sizeof(ext_shoot_data_t));
    memset(&game_info.bullet_remaining_t, 0, sizeof(ext_bullet_remaining_t));
	memset(&game_info.rfid_status, 0, sizeof(ext_rfid_status_t));
	memset(&game_info.dart_client_cmd_t, 0, sizeof(ext_dart_client_cmd_t));

    memset(&game_info.custom_data, 0, sizeof(ext_student_interactive_data_t));
    memset(&game_info.custom_data_robot, 0, sizeof(robot_interactive_data_t));
	memset(&game_info.robot_command_t, 0, sizeof(ext_robot_command_t));
	memset(&game_info.robot_version_t, 0, sizeof(ext_robot_version_t));
	memset(&game_info.map_command_r, 0, sizeof(ext_client_map_command_r));
		 //复制字符 c（一个无符号字符）到参数 str 所指向的字符串的前 n 个字符
		 //用第二个字符代替第一个变量的前（第三个变量）的字符
		 //sizeof实际上是获取了数据在内存中所占用的存储空间，以字节为单位来计数。
		 
		 
		 //作用： 实质上事就是对结构体数据所在的初始化    

}



//裁判系统数据处理
//此函数用于单字节解包最后的数据转移
// memcpy: 存储区 str2(第二个变量) 复制 n (第三个变量)个字节到存储区 str1(第一个变量)

uint16_t cmd_id = 0;
void referee_data_solve(uint8_t *frame)
{

    uint8_t index = 0;

    memcpy(&referee_receive_header, frame, sizeof(frame_header_structUI_t));    
																																						  
    index += sizeof(frame_header_structUI_t);

    memcpy(&cmd_id, frame + index, sizeof(uint16_t));
    index += sizeof(uint16_t);

    switch (cmd_id)
    {
        case GAME_STATE_CMD_ID:
        {
            memcpy(&game_info.game_state, frame + index, sizeof(ext_game_state_t));
        }
        break;
        case GAME_RESULT_CMD_ID:
        {
            memcpy(&game_info.game_result, frame + index, sizeof(ext_game_result_t));
        }
        break;
        case GAME_ROBOT_HP_CMD_ID:
        {
            memcpy(&game_info.game_robot_HP_t, frame + index, sizeof(ext_game_robot_HP_t));
        }
        break;
        case DART_STATUS_ID:
		{
            memcpy(&game_info.dart_status, frame + index, sizeof(ext_dart_status_t));				
		}
		break;
		case ICRA_BUFF_ID:
        {
            memcpy(&game_info.ICRA_buff, frame + index, sizeof(ext_ICRA_buff_debuff_zone_status_t));				
		}
		break;
        case FIELD_EVENTS_CMD_ID:
        {
            memcpy(&game_info.event_data, frame + index, sizeof(ext_event_data_t));
        }
        break;
        case SUPPLY_PROJECTILE_ACTION_CMD_ID:
        {
            memcpy(&game_info.supply_projectile_action, frame + index, sizeof(ext_supply_projectile_action_t));
        }
        break;
//        case SUPPLY_PROJECTILE_BOOKING_CMD_ID:
//        {
//            memcpy(&supply_projectile_booking_t, frame + index, sizeof(supply_projectile_booking_t));
//        }
//        break;
        case REFEREE_WARNING_CMD_ID:
        {
            memcpy(&game_info.referee_warning_t, frame + index, sizeof(ext_referee_warning_t));
        }
        break;
		case DART_REMAINING_TIME_ID:
		{
            memcpy(&game_info.dart_remaining_time_t, frame + index, sizeof(ext_dart_remaining_time_t));								
		}
		break;
        case ROBOT_STATE_CMD_ID:
        {
            memcpy(&game_info.game_robot_state, frame + index, sizeof(ext_game_robot_state_t));
        }
        break;
        case POWER_HEAT_DATA_CMD_ID:
        {
            memcpy(&game_info.power_heat_data, frame + index, sizeof(ext_power_heat_data_t));
						//Shoot_Heat_Limit();
		}
        break;
        case ROBOT_POS_CMD_ID:
        {
            memcpy(&game_info.game_robot_pos, frame + index, sizeof(ext_game_robot_pos_t));
        }
        break;
        case BUFF_MUSK_CMD_ID:
        {
            memcpy(&game_info.buff_musk, frame + index, sizeof(ext_buff_t));
        }
        break;
        case AERIAL_ROBOT_ENERGY_CMD_ID:
        {
            memcpy(&game_info.aerial_robot_energy, frame + index, sizeof(aerial_robot_energy_t));
        }
        break;
        case ROBOT_HURT_CMD_ID:
        {
            memcpy(&game_info.robot_hurt, frame + index, sizeof(ext_robot_hurt_t));
        }
        break;
        case SHOOT_DATA_CMD_ID:
        {
            memcpy(&game_info.shoot_data,frame + index, sizeof(ext_shoot_data_t));
//						if(shoot_data_t.bullet_speed>10 && shoot_data_t.bullet_type == SHELL_17MM)
//							Shoot_Data_Save(shoot_data_t.bullet_speed);
        }
        break;
        case BULLET_REMAINING_CMD_ID:
        {
            memcpy(&game_info.bullet_remaining_t, frame + index, sizeof(ext_bullet_remaining_t));
        }
        break;
		case RFID_STATUS_ID:
		{
            memcpy(&game_info.rfid_status, frame + index, sizeof(ext_rfid_status_t));								
		}
		break;
		case DART_CLIENT_CMD_ID:
		{
            memcpy(&game_info.dart_client_cmd_t, frame + index, sizeof(ext_dart_client_cmd_t));								
		}	
		break;			
// 机器人间通讯还未完善
//        case STUDENT_INTERACTIVE_DATA_CMD_ID:
//        {
//            memcpy(&game_info.custom_data, frame + index, sizeof(ext_student_interactive_data_t));
//        }
//        break;
		case ROBOT_INTERACTIVE_DATA_ID:
		{
            memcpy(&game_info.custom_data_robot, frame + index, sizeof(robot_interactive_data_t));								
		}
        break;
		case ROBOT_CMD_T_ID:
		{
            memcpy(&game_info.robot_command_t, frame + index, sizeof(ext_robot_command_t));								
		}
		break;					
		case ROBOT_VERSION_ID:						//键鼠信息
		{
            memcpy(&game_info.robot_version_t, frame + index, sizeof(ext_robot_version_t));								
		}
		break;
		case MAP_CMD_ID:
		{
            memcpy(&game_info.map_command_r, frame + index, sizeof(ext_client_map_command_r));								
		}
		break;
		default:
        {
            break;
        }
    }
}

////**********************************************************************************************
////为了避免移植太多文件，所以把一些小的函数放到了这里。
////**********************************************************************************************

void get_chassis_power_and_buffer(fp32 *power, fp32 *buffer)
{
    *power = game_info.power_heat_data.chassis_power;
    *buffer = game_info.power_heat_data.chassis_power_buffer;

}


uint8_t get_robot_id(void)
{
    return game_info.game_robot_state.robot_id;
}


void get_shoot_heat0_limit_and_heat0(uint16_t *heat0_limit, uint16_t *heat0)
{
//    *heat0_limit = game_info.game_robot_state.shooter_id1_17mm_cooling_limit;
    *heat0 = game_info.power_heat_data.shooter_id1_17mm_cooling_heat;
}


void get_shoot_heat1_limit_and_heat1(uint16_t *heat1_limit, uint16_t *heat1)
{
//    *heat1_limit = game_info.game_robot_state.shooter_id1_42mm_cooling_limit;
    *heat1 = game_info.power_heat_data.shooter_id1_42mm_cooling_heat;
}
uint16_t get_client_id(void)
{
    if (game_info.game_robot_state.robot_id < 100)
    {
        return 0x100 + game_info.game_robot_state.robot_id;
    }
    else
    {
        return 0x164 + (game_info.game_robot_state.robot_id - 100);
    }
}