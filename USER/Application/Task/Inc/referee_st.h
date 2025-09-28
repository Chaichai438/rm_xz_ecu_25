#ifndef REFEREE_H
#define REFEREE_H

#include "main.h"

#include "protocol.h"


//命令ID
enum Command_ID { 
	GAME_STATE = 0x0001, 
	GAME_RESULT, 
	GAME_ROBOT_SURVIVORS, 
	EVENT_DATA = 0x0101, 
	SUPPLY_PROJECTILE_ACTION, 
	SUPPLY_PROJECTILE_BOOKING, 
	GAME_ROBOT_STATE = 0x201, 
	POWER_HEAT_DATA, 
	GAME_ROBOT_POS, 
	BUFF_MASK, 
	ROBOT_ENERGY, 
	ROBOT_HURT, 
	SHOOT_DATA, 
	INTERACTIVE_DATA};

//机器人ID
//enum Robot_ID { RED_HERO = 1, RED_ENGINEERING, RED_INFANTRY_1, RED_INFANTRY_2, RED_INFANTRY_3, RED_AIR, RED_SENTRY,
//				BLUE_HERO = 11, BLUE_ENGINEERING, BLUE_INFANTRY_1, BLUE_INFANTRY_2, BLUE_INFANTRY_3, BLUE_AIR, BLUE_SENTRY};

				
//客户端ID
enum Client_ID { 
	CLIENT_RED_HERO = 0x0101, 
	CLIENT_RED_ENGINEERING, 
	CLIENT_RED_INFANTRY_1, 
	CLIENT_RED_INFANTRY_2, 
	CLIENT_RED_INFANTRY_3, 
	CLIENT_RED_AIR, 
	CLIENT_RED_SENTRY,
	CLIENT_BLUE_HERO = 0x0111, 
	CLIENT_BLUE_ENGINEERING, 
	CLIENT_BLUE_INFANTRY_1, 
	CLIENT_BLUE_INFANTRY_2, 
	CLIENT_BLUE_INFANTRY_3, 
	CLIENT_BLUE_AIR, 
	CLIENT_BLUE_SENTRY};

//机器人等级
enum Robot_Level { 
	ROBOT_LEVEL_1 = 1, 
	ROBOT_LEVEL_2, 
	ROBOT_LEVEL_3};

//比赛类型
enum Game_Type { 
	RM_COMBAT, 
	RM_INDIVIDUAL_EVENT, 
	RM_ICRA};

//比赛阶段
//enum Game_Progress { 
//	NOT_START, 
//	READY, 
//	SELF_CHECKING, 
//	COUNT_DOWN_5S, 
//	BATTLING, 
//	GAME_SETTLING};
	
//比赛结果
enum Game_Result { 
	GAME_TIE, 
	WINNER_RED, 
	WINNER_BLUE};

//增益类型
enum BUFF_Type { 
	BLOOD_SUPPYING, 
	MUZZLE_COOLING, 
	DEFENSE_UP, 
	ATTACK_UP};

//伤害类型
enum Hurt_Type { 
	ARMOR_HURTED, 
	MODULE_OFFLINE,
	SHOOTER_OVERHEAT, 
	CHASSIS_OVERPOWER};		//血量变化类型

//装甲类型
enum Armor_Type { 
	ARMOR_FRONT, 
	ARMOR_LEFT, 
	ARMOR_BEHIND, 
	ARMOR_RIGHT, ARMOR_UP};		//装甲ID(除哨兵)

//子弹类型
enum Bullet_Type { 
	SHELL_17MM = 1, 
	SHELL_42MM};

typedef enum
{
	   //红方
    RED_HERO        = 1,            //英雄
    RED_ENGINEER    = 2,            //工程
    RED_STANDARD_1  = 3,
    RED_STANDARD_2  = 4,             //步兵 
    RED_STANDARD_3  = 5,
    RED_AERIAL      = 6,            //空中
    RED_SENTRY      = 7,            //哨兵
    RED_Radar_station = 9,	        //雷达站
    //蓝方
  	BLUE_HERO       = 101,
    BLUE_ENGINEER   = 102,
    BLUE_STANDARD_1 = 103,
    BLUE_STANDARD_2 = 104,
    BLUE_STANDARD_3 = 105,
    BLUE_AERIAL     = 106,
    BLUE_SENTRY     = 107,
	BILE_Radar_station = 109,
} robot_id_t;

typedef enum                   //当前比赛阶段
{
	PROGRESS_Game_not_started = 0,      //未开始比赛
	PROGRESS_Preparation_stage = 1,      //准备阶段
	PROGRESS_Self_check_stage  =2,       //自检阶段
    PROGRESS_5sCOUNTDOWN    = 3,         //5S倒计时
    PROGRESS_In_the_play         = 4,     //对战中
    PROGRESS_Competition_Settlement    = 5,     //比赛结束中
} game_progress_t;                              //备注：协议附录里找不到


/**********************************结构体定义*********************************************************/


typedef __packed struct   //0x0001   比赛状态数据
{
    uint8_t game_type : 4;
    uint8_t game_progress : 4;
    uint16_t stage_remain_time;
	  uint64_t SyncTimeStamp;
} ext_game_state_t;


typedef __packed struct   //0x0002   比赛结果数据
{
    uint8_t winner;
} ext_game_result_t;


typedef __packed struct   //0x0003   机器人血量数据
{
	uint16_t red_1_robot_HP; //yx
	uint16_t red_2_robot_HP; //gc
	uint16_t red_3_robot_HP; //bb
	uint16_t red_4_robot_HP; //bb
	uint16_t reserved0; 	 //保留
	uint16_t red_7_robot_HP; //sb
	uint16_t red_outpost_HP; //前哨站
	uint16_t red_base_HP; 	 //基地

	uint16_t blue_1_robot_HP; 
	uint16_t blue_2_robot_HP; 
	uint16_t blue_3_robot_HP; 
	uint16_t blue_4_robot_HP; 
	uint16_t reserved1; 
	uint16_t blue_7_robot_HP; 
	uint16_t blue_outpost_HP;
	uint16_t blue_base_HP;
	
} ext_game_robot_HP_t;



typedef __packed struct //0x0101   场地时间数据
{
    uint32_t event_type;
} ext_event_data_t;




typedef __packed struct   //0104 裁判警告信息 
{
    uint8_t level;
    uint8_t offending_robot_id; 
 	uint8_t count;
} ext_referee_warning_t;

typedef __packed struct   //0x105  飞镖发射口倒计时
{
	uint8_t dart_remaining_time; 
	uint16_t dart_info;
} ext_dart_info_t;


typedef __packed struct   //0x0201  比赛机器人状态
{
	 uint8_t robot_id; 
  uint8_t robot_level; 
  uint16_t current_HP;  
  uint16_t maximum_HP; 
  uint16_t shooter_barrel_cooling_value; 
  uint16_t shooter_barrel_heat_limit; 
  uint16_t chassis_power_limit;  
  uint8_t power_management_gimbal_output : 1; 
  uint8_t power_management_chassis_output : 1;  
  uint8_t power_management_shooter_output : 1; 
} ext_robot_state_t;

typedef __packed struct  //0x0202   实时功率数据
{
	uint16_t reserved0; 
	uint16_t reserved1; 
	float reserved2; 
	uint16_t buffer_energy; 
	uint16_t shooter_17mm_1_barrel_heat; 
	uint16_t shooter_17mm_2_barrel_heat; 
	uint16_t shooter_42mm_barrel_heat;
} ext_power_heat_data_t;

typedef __packed struct  //0x0203  机器人位置
{
    float x;
    float y;
    float angle;
} ext_robot_pos_t;

typedef __packed struct //0x0204    机器人增益
{
 	uint8_t recovery_buff; 
 	uint8_t cooling_buff; 
 	uint8_t defence_buff; 
 	uint8_t vulnerability_buff; 
 	uint16_t attack_buff; 
 	uint8_t remaining_energy;
} ext_buff_t;


typedef __packed struct //0x0206   伤害状态
{
	uint8_t armor_id : 4; 
	uint8_t HP_deduction_reason : 4;
} ext_robot_hurt_t;


typedef __packed struct //0x0207   实时射击信息
{
 uint8_t bullet_type;
uint8_t shooter_id;
 uint8_t bullet_freq;
 float bullet_speed;
} ext_shoot_data_t;


typedef __packed struct //0x0208   子弹剩余发射数 
{
  uint16_t bullet_remaining_num_17mm;
  uint16_t bullet_remaining_num_42mm; 
  uint16_t coin_remaining_num;
} ext_bullet_remaining_t;

typedef __packed struct //0X0209  机器人RFID状态
{
   uint32_t rfid_status;
} ext_rfid_status_t;

typedef __packed struct   //0x020A 飞镖机器人客户端指令数据 
{
 uint8_t dart_launch_opening_status;
 uint8_t reserved;
 uint16_t target_change_time;
 uint16_t latest_launch_cmd_time;
} ext_dart_client_cmd_t;

typedef __packed struct //0x20B  地面机器人位置
{ 
 float hero_x; 
 float hero_y; 
 float engineer_x; 
 float engineer_y; 
 float standard_3_x; 
 float standard_3_y; 
 float standard_4_x; 
 float standard_4_y; 
 float reserved0; 
 float reserved1; 
}ext_ground_robot_position_t;

typedef __packed struct //0x20C 易伤情况
{ 
 uint8_t mark_progress;
}radar_mark_data_t;

typedef __packed struct   //0x20D 烧饼信息
{ 
uint32_t sentry_info; 
 uint16_t sentry_info_2; 
} sentry_info_t;


typedef __packed struct //0X20E 雷达信息
{ 
 uint8_t radar_info; 
} radar_info_t;
//*************************************************************************
//机器人间交互数据（内容还不完善）
//*************************************************************************


typedef __packed struct //0x301交互数据接收信息：
{
 uint16_t data_cmd_id;
 uint16_t sender_ID;
 uint16_t receiver_ID;
 uint8_t user_data[121];//可为数组，最大为122
}ext_student_interactive_data_t;

typedef __packed struct //子内容0x100图层操作
{ 

uint8_t delete_type; 

uint8_t layer; 

}interaction_layer_delete_t;

typedef __packed struct			//图形数据
{
	uint8_t graphic_name[3];	//图形名称
	uint32_t operate_tpye:3;	//操作类型
	uint32_t graphic_tpye:3;	//图形类型
	uint32_t layer:4;			//层数
	uint32_t color:4;			//颜色
	uint32_t start_angle:9;		
	uint32_t end_angle:9;		
	uint32_t width:10;			
	uint32_t start_x:11;		
	uint32_t start_y:11;		
	uint32_t radius:10;
	uint32_t end_x:11;
	uint32_t end_y:11;
} graphic_data_struct_t;	

typedef __packed struct //0x120 烧饼决策
{ 
 uint32_t sentry_cmd; 
} sentry_cmd_t;

typedef __packed struct //0x120 雷达决策
{ 
 uint8_t radar_cmd; 
} radar_cmd_t;



typedef struct
{
   uint16_t Data_ID;               //内容ID
   uint16_t Sender_ID;             //发送者ID
   uint16_t Receiver_ID;           //接收者ID
} UI_Data_Operate;         //操作定义帧


typedef __packed struct
{
    graphic_data_struct_t Graph_Control;
    uint8_t show_Data[30];
} String_Data;                  //打印字符串数据



typedef __packed struct              //客户端绘制一个图形 机器人间通信：0x0301 zi:0x101
{
	graphic_data_struct_t grapic_data_struct;
} ext_client_custom_graphic_single_t;

typedef __packed struct         //客户端绘制二个图形 机器人间通信：0x0301 zi 0x102
{
	graphic_data_struct_t grapic_data_struct[2];
} ext_client_custom_graphic_double_t;

typedef __packed struct             //客户端绘制五个图形 机器人间通信：0x03 0x103
{
	graphic_data_struct_t grapic_data_struct[5];
} ext_client_custom_graphic_five_t;

typedef __packed struct                    //客户端绘制七个图形 机器人间通信：0x0301
{
	graphic_data_struct_t grapic_data_struct[7];
} ext_client_custom_graphic_seven_t;


typedef __packed struct                   //客户端绘制字符 机器人间通信：0x0301
{
	graphic_data_struct_t grapic_data_struct;
	uint8_t data[30];
} ext_client_custom_character_t;

typedef __packed struct //0x302自定义控制器
{ 
uint8_t data[30]; 
}custom_robot_data_t;

typedef __packed struct 			//map_down客户端下发数据0x303
{ 
float target_position_x; 
float target_position_y; 
uint8_t cmd_keyboard; 
uint8_t target_robot_id; 
uint16_t cmd_source; 
}map_command_t;

typedef __packed struct //0x304 键鼠信息标识
{ 
int16_t mouse_x; 
int16_t mouse_y; 
int16_t mouse_z; 
int8_t left_button_down; 
int8_t right_button_down; 
uint16_t keyboard_value; 
uint16_t reserved; 
}remote_control_t;

typedef __packed struct 		//回传数据0x305
{
uint16_t hero_position_x; 
 uint16_t hero_position_y; 
 uint16_t engineer_position_x; 
 uint16_t engineer_position_y; 
 uint16_t infantry_3_position_x; 
 uint16_t infantry_3_position_y; 
 uint16_t infantry_4_position_x; 
 uint16_t infantry_4_position_y; 
 uint16_t infantry_5_position_x; 
 uint16_t infantry_5_position_y; 
 uint16_t sentry_position_x; 
 uint16_t sentry_position_y; 
} map_robot_data_t;

typedef __packed struct //0x306 非链路数据
{ 
uint16_t key_value; 
 uint16_t x_position:12; 
 uint16_t mouse_left:4; 
 uint16_t y_position:12;
 uint16_t mouse_right:4; 
 uint16_t reserved; 
}custom_client_data_t;


typedef __packed struct 		//0x307 
{ 
uint8_t intention; 
uint16_t start_position_x; 
uint16_t start_position_y; 
int8_t delta_x[49]; 
int8_t delta_y[49]; 
uint16_t sender_id; 
}map_data_t;

typedef __packed struct 		//0x308
{ 
uint16_t sender_id; 
uint16_t receiver_id; 
uint8_t user_data[30]; 
} custom_info_t;


typedef __packed struct
{
    uint8_t graphic_name[3];
    uint32_t operate_tpye : 3;
    uint32_t graphic_tpye : 3;
    uint32_t layer : 4;
    uint32_t color : 4;
    uint32_t start_angle : 9;
    uint32_t end_angle : 9;
    uint32_t width : 10;
    uint32_t start_x : 11;
    uint32_t start_y : 11;
    uint32_t radius : 10;
    uint32_t end_x : 11;
    uint32_t end_y : 11;              //图形数据
} Graph_Data;
typedef __packed struct			//浮点数据
{
	uint8_t graphic_name[3];
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t word_size:9;		//字体大小
	uint32_t significant_bit:9;		//小数位有效个数 
	uint32_t width:10;
	uint32_t start_x:11;        
	uint32_t start_y:11;
	float graph_Float;              //浮点数据
	/*
	uint32_t radius:10;				//以下三段 乘以1000后以32位整形数，int32_t
	uint32_t end_x:11;
	uint32_t end_y:11;
	*/
} Float_Data;


typedef __packed struct          //客户端删除图形 0301
{
uint8_t operate_tpye; 
uint8_t layer; 
} ext_client_custom_graphic_delete_t;


typedef __packed struct
{
	ext_game_state_t						game_state;							//比赛状态数据		(0x0001)
	ext_game_result_t						game_result;						//比赛结果数据		(0x0002)
	ext_game_robot_HP_t						game_robot_HP_t;					//机器人血量数据	(0x0003)
	// ext_dart_status_t						dart_status;						//飞镖状态数据		(0x0004)
	// ext_ICRA_buff_debuff_zone_status_t		ICRA_buff;							//人工智能挑战赛加成与惩罚区状态(0x0005)
		
	ext_event_data_t						event_data;							//场地事件数据场地时间数据		(0x0101)  发送频率：事件改变后发送 
	//	ext_supply_projectile_action_t			supply_projectile_action;			//补给站动作标识	(0x0102)  发送频率：动作改变后发送 
	//	ext_supply_projectile_booking_t			supply_projectile_booking;			//补给站预约子弹	(0x0103)  发送频率：上限 10Hz。RM 对抗赛尚未开放 
	ext_referee_warning_t					referee_warning_t;					//裁判警告信息 		(0x0104)
	ext_dart_info_t							dart_remaining_time_t;				//飞镖发射口倒计时	(0x0105)
	
	ext_robot_state_t						game_robot_state;					//比赛机器人状态	(0x0201)  发送频率：10Hz 
	ext_power_heat_data_t					power_heat_data;					//实时功率热量数据	(0x0202)  发送频率：50Hz 
	ext_robot_pos_t							game_robot_pos;						//机器人位置		(0x0203)  发送频率：10Hz
	ext_buff_t							    buff_musk;							//机器人增益		(0x0204)  发送频率：状态改变后发送 
	//aerial_robot_energy_t					aerial_robot_energy;				//空中机器人能量状态(0x0205)  发送频率：10Hz
	ext_robot_hurt_t						robot_hurt;							//伤害状态			(0x0206)  发送频率：伤害发生后发送 
	ext_shoot_data_t						shoot_data;							//实时射击信息		(0x0207)  发送频率：射击后发送 
	ext_bullet_remaining_t					bullet_remaining_t;					//子弹剩余发射数 	(0x0208)
	ext_rfid_status_t						rfid_status;						//机器人RFID状态	(0x0209)
	ext_dart_client_cmd_t					dart_client_cmd_t;					//飞镖机器人客户端指令数据	(0x020A)

	ext_ground_robot_position_t             round_robot_position;				//地面机器人位置
	radar_mark_data_t						radar_mark_data;					//雷达标记
	sentry_info_t							sentry_info;						//烧饼信息
	radar_info_t							radar_info;							//雷达信息

	ext_student_interactive_data_t			custom_data;						//交互数据接收信息（头）(0x0301)发送频率：10Hz  内容 ID:0x0200~0x02FF 
	custom_robot_data_t						custom_robot_data;					//自定义控制器		(0x0302)
	map_command_t							map_command;						//地图下发指令		(0x0303)		
	remote_control_t						remote_control;					//图传遥控信息标识   (0x0304)    图传下发,发送频率：30Hz。
	map_robot_data_t						map_robot_data;						//回传数据 (0x0305)
	map_data_t								map_data;							//地图数据			(0x0307)
	custom_info_t							custom_info;						//自定义信息		(0x0308)
} Game_info;


extern void init_referee_struct_data(void);
extern void referee_data_solve(uint8_t *frame);

extern void get_chassis_power_and_buffer(fp32 *power, fp32 *buffer);
extern Game_info game_info;	//裁判系统数据
extern uint8_t get_robot_id(void);
extern uint16_t get_client_id(void);

extern void get_shoot_heat0_limit_and_heat0(uint16_t *heat0_limit, uint16_t *heat0);
extern void get_shoot_heat1_limit_and_heat1(uint16_t *heat1_limit, uint16_t *heat1);
#endif
