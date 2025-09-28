#ifndef REFEREE_H
#define REFEREE_H

#include "main.h"

#include "protocol.h"


//????ID
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

//??????ID
//enum Robot_ID { RED_HERO = 1, RED_ENGINEERING, RED_INFANTRY_1, RED_INFANTRY_2, RED_INFANTRY_3, RED_AIR, RED_SENTRY,
//				BLUE_HERO = 11, BLUE_ENGINEERING, BLUE_INFANTRY_1, BLUE_INFANTRY_2, BLUE_INFANTRY_3, BLUE_AIR, BLUE_SENTRY};

				
//?????ID
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

//????????
enum Robot_Level { 
	ROBOT_LEVEL_1 = 1, 
	ROBOT_LEVEL_2, 
	ROBOT_LEVEL_3};

//????????
enum Game_Type { 
	RM_COMBAT, 
	RM_INDIVIDUAL_EVENT, 
	RM_ICRA};

//???????
//enum Game_Progress { 
//	NOT_START, 
//	READY, 
//	SELF_CHECKING, 
//	COUNT_DOWN_5S, 
//	BATTLING, 
//	GAME_SETTLING};
	
//???????
enum Game_Result { 
	GAME_TIE, 
	WINNER_RED, 
	WINNER_BLUE};

//????????
enum BUFF_Type { 
	BLOOD_SUPPYING, 
	MUZZLE_COOLING, 
	DEFENSE_UP, 
	ATTACK_UP};

//???????
enum Hurt_Type { 
	ARMOR_HURTED, 
	MODULE_OFFLINE,
	SHOOTER_OVERHEAT, 
	CHASSIS_OVERPOWER};		//????Å£????

//???????
enum Armor_Type { 
	ARMOR_FRONT, 
	ARMOR_LEFT, 
	ARMOR_BEHIND, 
	ARMOR_RIGHT, ARMOR_UP};		//???ID(?????)

//???????
enum Bullet_Type { 
	SHELL_17MM = 1, 
	SHELL_42MM};

typedef enum
{
	   //??
    RED_HERO        = 1,            //???
    RED_ENGINEER    = 2,            //????
    RED_STANDARD_1  = 3,
    RED_STANDARD_2  = 4,             //???? 
    RED_STANDARD_3  = 5,
    RED_AERIAL      = 6,            //????
    RED_SENTRY      = 7,            //???
    RED_Radar_station = 9,	        //????
    //????
  	BLUE_HERO       = 101,
    BLUE_ENGINEER   = 102,
    BLUE_STANDARD_1 = 103,
    BLUE_STANDARD_2 = 104,
    BLUE_STANDARD_3 = 105,
    BLUE_AERIAL     = 106,
    BLUE_SENTRY     = 107,
	BILE_Radar_station = 109,
} robot_id_t;

typedef enum                   //??????????
{
	PROGRESS_Game_not_started = 0,      //¶ƒ???????
	PROGRESS_Preparation_stage = 1,      //??????
	PROGRESS_Self_check_stage  =2,       //?????
    PROGRESS_5sCOUNTDOWN    = 3,         //5S?????
    PROGRESS_In_the_play         = 4,     //?????
    PROGRESS_Competition_Settlement    = 5,     //??????????
} game_progress_t;                              //?????ßø?∏Î????????


typedef __packed struct   //0x0001   ??????????
{
    uint8_t game_type : 4;
    uint8_t game_progress : 4;
    uint16_t stage_remain_time;
	  uint64_t SyncTimeStamp;
} ext_game_state_t;


typedef __packed struct   //0x0002   ???????????
{
    uint8_t winner;
} ext_game_result_t;


typedef __packed struct   //0x0003   ?????????????
{
 uint16_t red_1_robot_HP;
 uint16_t red_2_robot_HP; 
 uint16_t red_3_robot_HP; 
 uint16_t red_4_robot_HP; 
 uint16_t red_5_robot_HP; 
 uint16_t red_7_robot_HP; 
 uint16_t red_outpost_HP;
 uint16_t red_base_HP; 
 uint16_t blue_1_robot_HP; 
 uint16_t blue_2_robot_HP; 
 uint16_t blue_3_robot_HP; 
 uint16_t blue_4_robot_HP; 
 uint16_t blue_5_robot_HP; 
 uint16_t blue_7_robot_HP; 
 uint16_t blue_outpost_HP;
 uint16_t blue_base_HP;
} ext_game_robot_HP_t;

typedef __packed struct      // 0x0004 ?????????      //ßø?∏Î???
{
 uint8_t dart_belong; 
 uint16_t stage_remaining_time; 
} ext_dart_status_t;

typedef __packed struct       //?????????????????????????0x0005
{
 uint8_t F1_zone_status:1;
 uint8_t F1_zone_buff_debuff_status:3; 
 uint8_t F2_zone_status:1;
 uint8_t F2_zone_buff_debuff_status:3; 
 uint8_t F3_zone_status:1;
 uint8_t F3_zone_buff_debuff_status:3; 
 uint8_t F4_zone_status:1;
 uint8_t F4_zone_buff_debuff_status:3; 
 uint8_t F5_zone_status:1;
 uint8_t F5_zone_buff_debuff_status:3; 
 uint8_t F6_zone_status:1;
 uint8_t F6_zone_buff_debuff_status:3;
 uint16_t red1_bullet_left;
 uint16_t red2_bullet_left;
uint16_t blue1_bullet_left;
uint16_t blue2_bullet_left;
} ext_ICRA_buff_debuff_zone_status_t;

typedef __packed struct //0x0101   ???????????
{
    uint32_t event_type;
} ext_event_data_t;



typedef __packed struct  //0x0102   ????????????
{
 uint8_t supply_projectile_id; 
 uint8_t supply_robot_id; 
 uint8_t supply_projectile_step; 
 uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

typedef __packed struct   //0104 ???ß‡?????? 
{
    uint8_t level;
    uint8_t foul_robot_id;
} ext_referee_warning_t;

typedef __packed struct   //0x105  ????????????
{
 uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;


typedef __packed struct   //0x0201  ????????????
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
} ext_game_robot_state_t;

typedef __packed struct  //0x0202   ??????????
{
		 uint16_t chassis_volt; 
		 uint16_t chassis_current; 
		 float chassis_power; 
		 uint16_t chassis_power_buffer; 
		 uint16_t shooter_id1_17mm_cooling_heat;
		 uint16_t shooter_id2_17mm_cooling_heat;
		 uint16_t shooter_id1_42mm_cooling_heat;
} ext_power_heat_data_t;

typedef __packed struct  //0x0203  ??????¶À??
{
    float x;
    float y;
    float z;
    float yaw;
} ext_game_robot_pos_t;

typedef __packed struct //0x0204    ??????????
{
    uint8_t power_rune_buff;
} ext_buff_t;

typedef __packed struct //0x0205   ???ß›???????????
{
    uint8_t attack_time;
} aerial_robot_energy_t;

typedef __packed struct //0x0206   ?????
{
    uint8_t armor_type : 4;
    uint8_t hurt_type : 4;
} ext_robot_hurt_t;


typedef __packed struct //0x0207   ????????
{
 uint8_t bullet_type;
uint8_t shooter_id;
 uint8_t bullet_freq;
 float bullet_speed;
} ext_shoot_data_t;


typedef __packed struct //0x0208   ?????????? 
{
  uint16_t bullet_remaining_num_17mm;
  uint16_t bullet_remaining_num_42mm; 
  uint16_t coin_remaining_num;
} ext_bullet_remaining_t;

typedef __packed struct //0X0209  ??????RFID??
{
   uint32_t rfid_status;
} ext_rfid_status_t;

typedef __packed struct   //0x020A ???????????????????? 
{
 uint8_t dart_launch_opening_status;
 uint8_t dart_attack_target;
 uint16_t target_change_time;
 uint8_t first_dart_speed;
 uint8_t second_dart_speed;
 uint8_t third_dart_speed;
 uint8_t fourth_dart_speed;
 uint16_t last_dart_launch_time;
 uint16_t operate_launch_cmd_time;
} ext_dart_client_cmd_t;


//*************************************************************************
//??????Ÿ¨???????????????????
//*************************************************************************


typedef __packed struct //0x0301????????????????
{
 uint16_t data_cmd_id;
 uint16_t sender_ID;
 uint16_t receiver_ID;
} ext_student_interactive_data_t;


typedef __packed struct                //????????????????0x0302
{
	uint8_t data[512];                     //?????ßø?∏Î???? data[] ???????
} robot_interactive_data_t;


typedef __packed struct				//ß≥????°§?????????0x0303
{
	float target_position_x;
	float target_position_y;
	float target_position_z;
	uint8_t commd_keyboard;
	uint16_t target_robot_ID;
} ext_robot_command_t;
	
	
	
typedef __packed struct  //??????????????0x0304,????°§?
{
	int16_t mouse_x;
	int16_t mouse_y;
	int16_t mouse_z;
	int8_t left_button_down;
	int8_t right_button_down;
	uint16_t keyboard_value;
	uint16_t reserved;
} ext_robot_version_t;




typedef __packed struct		//ß≥???????????????0x0305??
{
uint16_t target_robot_ID;
float target_position_x;
float target_position_y;
} ext_client_map_command_r;


typedef __packed struct          //??????????? 0301
{
uint8_t operate_tpye; 
uint8_t layer; 
} ext_client_custom_graphic_delete_t;



////?????UI
//typedef struct
//{
//   u8 SOF;                    //??????,???0xA5
//   u16 Data_Length;           //????????
//   u8 Seq;                    //?????
//   u8 CRC8;                   //CRC8ßµ???
//   u16 CMD_ID;                //????ID	//0x301
//} frame_header_struct_t;             //??


typedef struct
{
   u16 Data_ID;               //????ID
   u16 Sender_ID;             //??????ID
   u16 Receiver_ID;           //??????ID
} UI_Data_Operate;         //?????????

//typedef struct
//{ 
//   uint8_t graphic_name[3]; 
//   uint32_t operate_tpye:3; 
//   uint32_t graphic_tpye:3; 
//   uint32_t layer:4; 
//   uint32_t color:4; 
//   uint32_t start_angle:9;
//   uint32_t end_angle:9;
//   uint32_t width:10; 
//   uint32_t start_x:11; 
//   uint32_t start_y:11;
//   float graph_Float;              //????????
//} Float_Data;




typedef __packed struct			//???????
{
	uint8_t graphic_name[3];	//???????
	uint32_t operate_tpye:3;	//????????
	uint32_t graphic_tpye:3;	//???????
	uint32_t layer:4;			//????
	uint32_t color:4;			//???
	uint32_t start_angle:9;		
	uint32_t end_angle:9;		
	uint32_t width:10;			
	uint32_t start_x:11;		
	uint32_t start_y:11;		
	uint32_t radius:10;
	uint32_t end_x:11;
	uint32_t end_y:11;
} graphic_data_struct_t;		

typedef __packed struct			//????????
{
	uint8_t graphic_name[3];
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t word_size:9;		//?????ß≥
	uint32_t significant_bit:9;		//ß≥??¶À??ßπ???? 
	uint32_t width:10;
	uint32_t start_x:11;        
	uint32_t start_y:11;
	float graph_Float;              //????????
	/*
	uint32_t radius:10;				//???????? ????1000????32¶À????????int32_t
	uint32_t end_x:11;
	uint32_t end_y:11;
	*/
} Float_Data;

//typedef struct
//{ 
//   uint8_t graphic_name[3]; 
//   uint32_t operate_tpye:3; 
//   uint32_t graphic_tpye:3; 
//   uint32_t layer:4; 
//   uint32_t color:4; 
//   uint32_t start_angle:9;
//   uint32_t end_angle:9;
//   uint32_t width:10; 
//   uint32_t start_x:11; 
//   uint32_t start_y:11;
//   
//} Float_Data;

typedef __packed struct			//????????
{
	uint8_t graphic_name[3];
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t word_size:9;		//?????ß≥
	uint32_t significant_bit:9;		//?? 
	uint32_t width:10;				//???????
	uint32_t start_x:11;        
	uint32_t start_y:11;
	uint32_t radius:10;				//???????? 32¶À????????int32_t
	uint32_t end_x:11;
	uint32_t end_y:11;
} int_data_struct_t;

typedef __packed struct			//???????
{
	uint8_t graphic_name[3];
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t word_size:9;		//???????
	uint32_t significant_bit:9;		//?? 
	uint32_t width:10;
	uint32_t start_x:11;        
	uint32_t start_y:11;
	uint32_t radius:10;				//???????????
	uint32_t end_x:11;
	uint32_t end_y:11;
} string_data_struct_t;

//typedef struct
//{ 
//   uint8_t graphic_name[3]; 
//   uint32_t operate_tpye:3; 
//   uint32_t graphic_tpye:3; 
//   uint32_t layer:4; 
//   uint32_t color:4; 
//   uint32_t start_angle:9;
//   uint32_t end_angle:9;
//   uint32_t width:10; 
//   uint32_t start_x:11; 
//   uint32_t start_y:11;
//   float graph_Float;              //????????
//} Float_Data;




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
    uint32_t end_y : 11;              //???????
} Graph_Data;

typedef __packed struct
{
    Graph_Data Graph_Control;
    uint8_t show_Data[30];
} String_Data;                  //????????????

/*
typedef struct
{ 
   uint8_t graphic_name[3]; 
   uint32_t operate_tpye; 
   uint32_t graphic_tpye; 
   uint32_t layer; 
   uint32_t color; 
   uint32_t start_angle;
   uint32_t end_angle;
   uint32_t width; 
   uint32_t start_x; 
   uint32_t start_y;
   float graph_Float;              //????????
} Float_Data;
*/   

typedef __packed struct              //?????????????? ???????????0x0301
{
	graphic_data_struct_t grapic_data_struct;
} ext_client_custom_graphic_single_t;

typedef __packed struct         //?????????????? ???????????0x0301
{
	graphic_data_struct_t grapic_data_struct[2];
} ext_client_custom_graphic_double_t;

typedef __packed struct             //?????????????? ???????????0x0301
{
	graphic_data_struct_t grapic_data_struct[5];
} ext_client_custom_graphic_five_t;

typedef __packed struct                    //?????????????? ???????????0x0301
{
	graphic_data_struct_t grapic_data_struct[7];
} ext_client_custom_graphic_seven_t;


typedef __packed struct                   //??????????? ???????????0x0301
{
	graphic_data_struct_t grapic_data_struct;
	uint8_t data[30];
} ext_client_custom_character_t;



/*
typedef __packed struct
{
    float data1;
    float data2;
    float data3;
    uint8_t data4;
} custom_data_t;


typedef __packed struct
{
    uint8_t data[64];
} ext_up_stream_data_t;

typedef __packed struct
{
    uint8_t data[32];
} ext_download_stream_data_t;

*/

typedef __packed struct
{
	ext_game_state_t						game_state;							//??????????		(0x0001)
	ext_game_result_t						game_result;						//???????????		(0x0002)
	ext_game_robot_HP_t						game_robot_HP_t;					//?????????????	(0x0003)
	ext_dart_status_t						dart_status;						//??????????		(0x0004)
	ext_ICRA_buff_debuff_zone_status_t		ICRA_buff;							//???????????????????????(0x0005)
		
	ext_event_data_t						event_data;							//?????????????????????		(0x0101)  ????????????????? 
	ext_supply_projectile_action_t			supply_projectile_action;			//????????????	(0x0102)  ?????????????????? 
//	ext_supply_projectile_booking_t			supply_projectile_booking;			//??????????	(0x0103)  ???????????? 10Hz??RM ???????¶ƒ???? 
	ext_referee_warning_t					referee_warning_t;					//???ß‡?????? 		(0x0104)
	ext_dart_remaining_time_t				dart_remaining_time_t;				//????????????	(0x0105)
	
	ext_game_robot_state_t					game_robot_state;					//????????????	(0x0201)  ????????10Hz 
	ext_power_heat_data_t					power_heat_data;					//??????????????	(0x0202)  ????????50Hz 
	ext_game_robot_pos_t					game_robot_pos;						//??????¶À??		(0x0203)  ????????10Hz
	ext_buff_t							    buff_musk;							//??????????		(0x0204)  ???????????????? 
	aerial_robot_energy_t					aerial_robot_energy;				//???ß›???????????(0x0205)  ????????10Hz
	ext_robot_hurt_t						robot_hurt;							//?????			(0x0206)  ??????????????????? 
	ext_shoot_data_t						shoot_data;							//????????		(0x0207)  ??????????????? 
	ext_bullet_remaining_t					bullet_remaining_t;					//?????????? 	(0x0208)
	ext_rfid_status_t						rfid_status;						//??????RFID??	(0x0209)
	ext_dart_client_cmd_t					dart_client_cmd_t;					//????????????????????	(0x020A)

	ext_student_interactive_data_t			custom_data;						//???????????????????(0x0301)????????10Hz  ???? ID:0x0200~0x02FF 
	robot_interactive_data_t				custom_data_robot;					//??????????????   (0x0302) ???????????? 30Hz
	ext_robot_command_t 					robot_command_t;					//ß≥????°§??????? (0x0303)  ??????????????????
	ext_robot_version_t						robot_version_t;					//????????????   (0x0304)    ????°§?,????????30Hz??
	ext_client_map_command_r				map_command_r;						//ß≥????????????? (0x0305)

} Game_info;




extern void init_referee_struct_data(void);
extern void referee_data_solve(uint8_t *frame);

extern void get_chassis_power_and_buffer(fp32 *power, fp32 *buffer);
extern Game_info game_info;	//??????????
extern uint8_t get_robot_id(void);
extern uint16_t get_client_id(void);

extern void get_shoot_heat0_limit_and_heat0(uint16_t *heat0_limit, uint16_t *heat0);
extern void get_shoot_heat1_limit_and_heat1(uint16_t *heat1_limit, uint16_t *heat1);
#endif
