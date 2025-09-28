#ifndef  __ST_VERSION_H
#define  __ST_VERSION_H
#include "struct_typedef.h"

#define  VERSION_DATA_LEN  14

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

void Version_Data_Send(void);
extern int gyro_yaw;
#endif