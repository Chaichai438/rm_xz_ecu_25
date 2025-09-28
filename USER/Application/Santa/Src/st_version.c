#include "st_version.h"
#include "gimbal.h"
#include "usart.h"
#include "referee_st.h"
#include "usbd_cdc_if.h"
#include "referee_usart_task_st.h"
#include"CRC8_CRC16.h"
#include "cmsis_os.h"
#define OPEN_BAIBUSHIJUE 0
#define OPEN_SHAOBINGSHIJUE 0
#include <stdint.h>
#include <string.h>




uint8_t VERSION_DATA[VERSION_DATA_LEN];
uint16_t time=0; //电控收到第一组数据到发出的时间差
uint8_t CTRL_STATE,VERSION_STATE;
uint8_t ch[2]="\r\n";
extern volatile float GYRO_YAW_ANGLE_GET_X,GYRO_PITCH_ANGLE_GET_X,GYRO_G_Z_XHH,GYRO_G_X_XHH;
extern Game_info game_info;						//裁判系统数据


extern int buff_auto_status;
int bullet_speed_gyro=0;
//int robot_color=0,robot_color_id=0;


//char robot_color=0,robot_color_id=0;

 int inforbuff;
union shijue_union pitch_shijue,yaw_shijue;
extern uint8_t referee_fifo_buf[REFEREE_FIFO_BUF_LENGTH];

extern void usart1_tx_dma_enable(uint8_t *data, uint16_t len);
#if OPEN_BAIBUSHIJUE
void Version_Data_Send(void)
{
	yaw_shijue.a=GYRO_YAW_ANGLE_GET_X;
	pitch_shijue.a=GYRO_PITCH_ANGLE_GET_X;
	bullet_speed_gyro=game_info.shoot_data.bullet_speed*100;
	robot_color=game_info.game_robot_state.robot_id;
		if(robot_color==103||robot_color==104||robot_color==105)		//蓝
	{
		robot_color_id=0;
	}
		if(robot_color==3||robot_color==4||robot_color==5)					//红
	{
		robot_color_id=1;
	}
	
	VERSION_DATA[0]=0x78;
	for(int i=0;i<4;i++)
		VERSION_DATA[i+1]=pitch_shijue.b[i];	
	for(int i=0;i<4;i++)
		VERSION_DATA[i+5]=yaw_shijue.b[i];
	
	VERSION_DATA[9]=(int16_t)bullet_speed_gyro>>8;
	VERSION_DATA[10]=(int16_t)bullet_speed_gyro;
	VERSION_DATA[11]=robot_color_id;
	VERSION_DATA[12]=inforbuff;		
	VERSION_DATA[13]=0x76;
	
	HAL_UART_Transmit(&huart1, (uint8_t *)&VERSION_DATA, 14, 0xffff);///<普通串口发送数据
//usart1_tx_dma_enable((uint8_t *)&VERSION_DATA,sizeof(VERSION_DATA));

}
#endif
#if OPEN_SHAOBINGSHIJUE
void Version_Data_Send(void)
{
//	yaw_shijue.a=GYRO_YAW_ANGLE_GET_X;
//	pitch_shijue.a=GYRO_PITCH_ANGLE_GET_X;
	yaw_shijue.a=(int)(GYRO_YAW_ANGLE_GET_X*100);
	pitch_shijue.a=(int)(GYRO_PITCH_ANGLE_GET_X*100);
	
	bullet_speed_gyro=game_info.shoot_data.bullet_speed*100;
	robot_color=game_info.game_robot_state.robot_id;
		if(robot_color==103||robot_color==104||robot_color==105)		//蓝
	{
		robot_color_id=1;
	}
		if(robot_color==3||robot_color==4||robot_color==5)					//红
	{
		robot_color_id=0;
	}
	
	VERSION_DATA[0]=0x78;
//VERSION_DATA[1]=robot_color_id;
VERSION_DATA[1]=0x03;
	
	for(int i=0;i<4;i++)
		VERSION_DATA[i+2]=pitch_shijue.b[i];
//	
//	VERSION_DATA[2]=pitch_shijue.b[3];
//	VERSION_DATA[3]=pitch_shijue.b[2];
//	VERSION_DATA[4]=pitch_shijue.b[1];
//	VERSION_DATA[5]=pitch_shijue.b[0];
//	
	
	for(int i=0;i<4;i++)
		VERSION_DATA[i+6]=yaw_shijue.b[i];
	
	VERSION_DATA[10]=0;             //时间戳
	VERSION_DATA[11]=0; 
	VERSION_DATA[12]=0;		
	VERSION_DATA[13]=0;
	
	VERSION_DATA[14]=(int16_t)bullet_speed_gyro>>8;
	VERSION_DATA[15]=(int16_t)bullet_speed_gyro;
	
	VERSION_DATA[16]=0;    //空位
	
	VERSION_DATA[17]=0;    //crc
	
//	VERSION_DATA[18]=inforbuff;		
	
//	VERSION_DATA[19]=0x76;
	
	HAL_UART_Transmit(&huart1, (uint8_t *)&VERSION_DATA, 14, 0xffff);///<普通串口发送数据
//usart1_tx_dma_enable((uint8_t *)&VERSION_DATA,sizeof(VERSION_DATA));

}


#define HEADER_SOF 0xA5
#define MAX_DATA_SIZE 128
#define CMD_ID_SIZE 2
#define FRAME_HEADER_SIZE 5
#define FRAME_TAIL_SIZE 2
#define MAX_FRAME_SIZE 128
extern Game_info game_info;	

// 主发送函数，发送三个指定的数据块
void send_selected_game_info() {
    //发送比赛状态数据 (game_state)
    Version_data_CDC_send(0x0001, (uint8_t*)&game_info.game_state, sizeof(game_info.game_state));

    //发送机器人血量(game_robot_HP_t)
   Version_data_CDC_send(0x0003, (uint8_t*)&game_info.game_robot_HP_t, sizeof(game_info.game_robot_HP_t));

    // 发送机器人受击 (game_robot_hurt)
    Version_data_CDC_send(0x0206, (uint8_t*)&game_info.robot_hurt, sizeof(game_info.robot_hurt));

	//发送机器人功率数据（power_heat_data）
	Version_data_CDC_send(0x0202, (uint8_t*)&game_info.power_heat_data, sizeof(game_info.power_heat_data));

}


void Version_data_CDC_send(uint16_t cmd_id, uint8_t *data, uint16_t data_len)
{
    uint8_t frame[MAX_FRAME_SIZE];
    // 打包数据帧
    uint16_t frame_len = pack_data_frame(frame, cmd_id, data, data_len);
	
		osDelay(100);
	CDC_Transmit_FS((uint8_t *)frame,frame_len);
}

uint8_t current_seq = 0;  // 包序号初始值为0

// 获取下一个包序号，并自动递增
uint8_t get_next_sequence() {
    uint8_t seq = current_seq;
    current_seq = (current_seq + 1) % 256;  // 递增后对256取模，确保 seq 在 0-255 之间循环
    return seq;
}

// 数据打包函数并发送
uint16_t pack_data_frame(uint8_t *frame, uint16_t cmd_id, uint8_t *data, uint16_t data_len) {
	frame_header_structUI_t referee_send_header;
    uint16_t frame_len = FRAME_HEADER_SIZE + CMD_ID_SIZE + data_len + FRAME_TAIL_SIZE;

    // 设置帧头
    referee_send_header.SOF = HEADER_SOF;
    referee_send_header.Data_Length= data_len;
    referee_send_header.Seq = get_next_sequence();  // 获取下一个序列号
    memcpy(frame, &referee_send_header, FRAME_HEADER_SIZE-1);

    // 计算并填充 CRC8
     append_CRC8_check_sum(frame, FRAME_HEADER_SIZE );

    // 填充 cmd_id 和数据段
    memcpy(frame + FRAME_HEADER_SIZE, &cmd_id, CMD_ID_SIZE);
    memcpy(frame + FRAME_HEADER_SIZE + CMD_ID_SIZE, data, data_len);

    // 计算并填充 CRC16
   append_CRC16_check_sum(frame, frame_len);
    return frame_len;  // 返回完整帧的长度
}
#endif


