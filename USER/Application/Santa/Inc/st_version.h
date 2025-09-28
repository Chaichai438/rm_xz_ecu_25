#ifndef  __ST_VERSION_H
#define  __ST_VERSION_H
#include "struct_typedef.h"

#define  VERSION_DATA_LEN  18

enum CTRL_State{
	NORMAL_STATE=0x11,		//跟随状态
	LITTEL_GYRO=0,		//小陀螺
	KEY_CTRL=1			//键鼠控制
};

enum VERSION_State{
	SELF_SCANNING=0x11,		//自瞄
	WINDMILL=0x22,			//大风车
};

union shijue_union//共用体
{
	float a;
	char b[4];
};


//union shijue_union//共用体
//{
//    int32_t a;         // 32位整型数据
//    uint8_t b[4];     // 对应的4字节数据
//};


void Version_Data_Send(void);
 uint16_t pack_data_frame(uint8_t *frame, uint16_t cmd_id, uint8_t *data, uint16_t data_len); //打包长度
void Version_data_CDC_send(uint16_t cmd_id, uint8_t *data, uint16_t data_len);	//发送数据

void send_selected_game_info();
extern int gyro_yaw;
#endif

