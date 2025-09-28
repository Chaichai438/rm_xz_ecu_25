#include "gimbal.h"
#include "can.h"
#include "pid_regulator.h"
#include "motor.h"
#include "remote_control.h"
#include "gyro.h"
#include "detect_task.h"
#include "st_version.h"
#include "usb_task.h"
#include "bsp_usart.h"
#include "gimbal_ctrl.h"

PID_Regulator_t PITCH_SPD_PID,YAW_SPD_PID;
PID_Regulator_t PITCH_POS_PID,YAW_POS_PID;
PID_Regulator_t PITCH_SPD_VISION_PID,YAW_SPD_VISION_PID;
PID_Regulator_t PITCH_POS_VISION_PID,YAW_POS_VISION_PID;

int distinguish=0;//是否识别到装甲板
gimbal_motor_back gimbal_motor_info[2];

extern int VISION_CNT;
int GIMBAL_CNT=0;

extern RC_ctrl_t rc_ctrl;
extern volatile float yaw_angle;                      //板载陀螺仪
extern volatile float mygetqval[9];	                  //用于存放传感器转换结果的数组
extern volatile float YAW_ANGLE_GET_X,G_Z_XHH;          //xhh
extern volatile float PITCH_ANGLE_GET_X,G_X_XHH;        //xhh
extern volatile float GYRO_YAW_ANGLE_GET_X,GYRO_G_Z_XHH;          //C板板载陀螺仪
extern volatile float GYRO_PITCH_ANGLE_GET_X,GYRO_G_X_XHH;       //C板板载陀螺仪
extern volatile float PITCH_VISION,YAW_VISION;

// 达妙4310电机实例
dm4310_motor_t pitch_dm4310 = {0};

// 达妙4310电机初始化
void DM4310_Init(void) {
    pitch_dm4310.mode = 0; // 默认位置模式
    pitch_dm4310.vel_set = 0;
    pitch_dm4310.pos_set = 0;
}

// 设置达妙4310电机速度
void DM4310_Set_Speed(float vel_set) {
    pitch_dm4310.vel_set = vel_set;
    pitch_dm4310.mode = 1; // 速度模式
}

// 设置达妙4310电机位置
void DM4310_Set_Position(float pos_set) {
    pitch_dm4310.pos_set = pos_set;
    pitch_dm4310.mode = 0; // 位置模式
}

// 启用达妙4310电机
void DM4310_Enable(void) {
    // 发送启用命令
    uint8_t enable_cmd[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC};
    CAN1_Send_Msg(0x05, enable_cmd, 8);
}

// 禁用达妙4310电机
void DM4310_Disable(void) {
    // 发送禁用命令
    uint8_t disable_cmd[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD};
    CAN1_Send_Msg(0x01, disable_cmd, 8);
}

void Gimbal_pid_init(void)
{	
	PITCH_SPD_PID.kp=45.0f;								//68.5f;                      //板载陀螺仪
	PITCH_SPD_PID.ki= 1.0f;								//1.0f;
	PITCH_SPD_PID.kd=0.65f;
	PITCH_SPD_PID.componentKpMax=25000.0f;
	PITCH_SPD_PID.componentKiMax=6000.0f;
	PITCH_SPD_PID.componentKdMax=6000.0f;
	PITCH_SPD_PID.outputMax=25000.0f;	
	PITCH_POS_PID.kp=35.0f;
	PITCH_POS_PID.ki=0.0001f;
	PITCH_POS_PID.kd=0.5f;
	PITCH_POS_PID.componentKpMax=10000.0f;
	PITCH_POS_PID.componentKiMax=6000.0f;
	PITCH_POS_PID.componentKdMax=6000.0f;
	PITCH_POS_PID.outputMax=10000.0f;

	YAW_SPD_PID.kp=50.0f;                       //xhh		110
	YAW_SPD_PID.ki=0.00f;    //0.001
	YAW_SPD_PID.kd=0.01f;
	YAW_SPD_PID.componentKpMax=25000.0f;
	YAW_SPD_PID.componentKiMax=6000.0f;
	YAW_SPD_PID.componentKdMax=6000.0f;
	YAW_SPD_PID.outputMax=10000.0f;	
	YAW_POS_PID.kp=30.0f;
	YAW_POS_PID.ki=0.003f;
	YAW_POS_PID.kd=0.03f;
	YAW_POS_PID.componentKpMax=10000.0f;
	YAW_POS_PID.componentKiMax=6000.0f;
	YAW_POS_PID.componentKdMax=6000.0f;
	YAW_POS_PID.outputMax=5000.0f;
	
	//视觉pid
	PITCH_POS_VISION_PID.kp=30.0f;         // 27
	PITCH_POS_VISION_PID.ki=0.13f;
	PITCH_POS_VISION_PID.kd=5.08f;
	PITCH_POS_VISION_PID.componentKpMax=10000.0f;
	PITCH_POS_VISION_PID.componentKiMax=6000.0f;
	PITCH_POS_VISION_PID.componentKdMax=8000.0f;
	PITCH_POS_VISION_PID.outputMax=10000.0f;
	PITCH_SPD_VISION_PID.kp=20.1f;                //127    
	PITCH_SPD_VISION_PID.ki=0.53f;
	PITCH_SPD_VISION_PID.kd=5.1f;
	PITCH_SPD_VISION_PID.componentKpMax=25000.0f;
	PITCH_SPD_VISION_PID.componentKiMax=6000.0f;
	PITCH_SPD_VISION_PID.componentKdMax=8000.0f;
	PITCH_SPD_VISION_PID.outputMax=25000.0f;	
	
	YAW_POS_VISION_PID.kp=65.0f;			//25.5 40  70
	YAW_POS_VISION_PID.ki=0.0f;  //0.13   0.19    1
	YAW_POS_VISION_PID.kd=0.0f;    //0.5   40   5
	YAW_POS_VISION_PID.componentKpMax=10000.0f;  
	YAW_POS_VISION_PID.componentKiMax=3000.0f;
	YAW_POS_VISION_PID.componentKdMax=8000.0f;
	YAW_POS_VISION_PID.outputMax=10000.0f;
	YAW_SPD_VISION_PID.kp=80.0f;    //79.9           58      90       
	YAW_SPD_VISION_PID.ki=0.0f;    //0.73   0.13     0.25
	YAW_SPD_VISION_PID.kd=0.0f;    //15. 5               43     5
	YAW_SPD_VISION_PID.componentKpMax=25000.0f;
	YAW_SPD_VISION_PID.componentKiMax=3000.0f;
	YAW_SPD_VISION_PID.componentKdMax=8000.0f;
	YAW_SPD_VISION_PID.outputMax=10000.0f;
	
	gimbal_motor_info[0].angle_set  = 0;
	gimbal_motor_info[1].angle_set  = 0;
	
	// 初始化达妙4310电机
	DM4310_Init();
	DM4310_Enable();
}

extern u8 ammunition_canbuf[8];
extern int quit_flag;
extern int vision_flag;
float pitch_test=0,yaw_test=0; 
int test_cnt=0;
extern volatile float PITCH_VISION_LAST,YAW_VISION_LAST;
extern int mode_channel,last_channel;
extern int round_count;

extern u8 ammunition_canbuf[8];

float yaw_change=0, pitch_change=0;
float tha0_yaw=0;
float tha_yaw=0;
float fact_yaw=0;
float point_yaw=0;
float last_yaw_set=0,last_pitch_set=0;
int buff_auto_status=0;																	//自瞄模式 风车1，装甲0	static
float pitch_temp,yaw_temp;	
int pitch_duandian=1;
u8 panduan_key=0;
float pitch_changes;
float target_pos = 0;

// PID重置函数
void PID_Regulator_Reset(PID_Regulator_t *pid)
{
    pid->err[0] = 0;
    pid->componentKp = 0;
    pid->componentKi = 0;
    pid->componentKd = 0;
    pid->componentKpMax = pid->componentKpMax;
    pid->componentKiMax = pid->componentKiMax;
    pid->componentKdMax = pid->componentKdMax;
    pid->output = 0;
}

// 仿照Yaw轴的控制方式，使用陀螺仪数据控制Pitch轴

void Gimbal_Motor_Speed_Set(float yaw_motor_pos, float pitch_motor_pos)
{
	short pid_out[2] = {0};
	u8 canbuf[8];
    static float pitch_target_angle = 0; // Pitch轴目标角度

	Version_Data_Send();

    // Yaw控制模式判断
    uint8_t yaw_vision_mode = (rc_ctrl.mouse.press_r == 1) || (rc_ctrl.rc.s[1] == 1);
    
    // Pitch控制模式判断
    uint8_t pitch_vision_mode = (rc_ctrl.rc.s[1] == PITCH_VISION_MODE);
    uint8_t pitch_remote_mode = (rc_ctrl.rc.s[1] == PITCH_REMOTE_MODE);

    // Yaw控制部分（大疆6020）- 保持不变
    if((rc_ctrl.mouse.press_r == 1) || (rc_ctrl.rc.s[1] == 1))  // 视觉模式
    {
        // 视觉模式PID计算
        PITCH_POS_VISION_PID.fdb = GYRO_PITCH_ANGLE_GET_X;
        PITCH_POS_VISION_PID.ref = PITCH_VISION;
        PID_Calc(&PITCH_POS_VISION_PID);
        pitch_temp = PITCH_POS_VISION_PID.output;
        
        YAW_POS_VISION_PID.fdb = GYRO_YAW_ANGLE_GET_X;
        YAW_POS_VISION_PID.ref = YAW_VISION;
        PID_Calc(&YAW_POS_VISION_PID);
        yaw_temp = YAW_POS_VISION_PID.output;
        
        // 速度环计算
        PITCH_SPD_VISION_PID.fdb = -GYRO_G_X_XHH;
        PITCH_SPD_VISION_PID.ref = -pitch_temp;
        PID_Calc(&PITCH_SPD_VISION_PID);
        
        YAW_SPD_VISION_PID.fdb = GYRO_G_Z_XHH;
        YAW_SPD_VISION_PID.ref = yaw_temp;
        PID_Calc(&YAW_SPD_VISION_PID);
        
        pid_out[0] = YAW_SPD_VISION_PID.output;
        pid_out[1] = PITCH_SPD_VISION_PID.output;
    }
    else  // 普通遥控模式
    {
        // 关键修复：当拨杆不在视觉模式时，使用当前位置作为设定值
        if(last_channel != mode_channel)
        {
            // 使用当前实际位置作为设定值，避免跳变
//            yaw_motor_pos = GYRO_YAW_ANGLE_GET_X;
//            pitch_motor_pos = GYRO_PITCH_ANGLE_GET_X;
            
            // 更新电机角度设定
            gimbal_motor_info[0].angle_set = yaw_motor_pos * 10;
            gimbal_motor_info[1].angle_set = pitch_motor_pos * 10;
        }
        
        // 位置环PID计算
        PITCH_POS_PID.fdb = GYRO_PITCH_ANGLE_GET_X;
        PITCH_POS_PID.ref = pitch_motor_pos;
        PID_Calc(&PITCH_POS_PID);
        pitch_temp = PITCH_POS_PID.output;
        
        YAW_POS_PID.fdb = GYRO_YAW_ANGLE_GET_X;
        YAW_POS_PID.ref = yaw_motor_pos;
        PID_Calc(&YAW_POS_PID);
        yaw_temp = YAW_POS_PID.output;
        
        // 速度环PID计算
        PITCH_SPD_PID.fdb = -GYRO_G_X_XHH;
        PITCH_SPD_PID.ref = -pitch_temp;
        PID_Calc(&PITCH_SPD_PID);
        
        YAW_SPD_PID.fdb = GYRO_G_Z_XHH;
        YAW_SPD_PID.ref = yaw_temp;
        PID_Calc(&YAW_SPD_PID);
        
        pid_out[0] = YAW_SPD_PID.output;
        pid_out[1] = PITCH_SPD_PID.output;
        
        last_channel = mode_channel;
    }		

  pid_out[1]*=pitch_duandian;
	if(toe_is_error(DBUS_TOE))
	{
		pid_out[0]=0;
		pid_out[1]=0;
	}
	
	//板载陀螺仪
	canbuf[0] = pid_out[0] >> 8;
	canbuf[1] = pid_out[0];
	canbuf[2] = 0;
	canbuf[3] = 0;
	canbuf[6] = 0;
	canbuf[7] = 0;
	//拨弹电机数据，在此发送避免控制错误（分开发送会有0和数据间隔发送，影响控制）
	canbuf[4] = ammunition_canbuf[4];
	canbuf[5] = ammunition_canbuf[5];
CAN2_Send_Msg( 0x1ff, canbuf, 8);		//发送数据

    // Pitch控制部分
    if(pitch_vision_mode) {
        // 视觉模式 - 使用视觉提供的角度
        pitch_target_angle = PITCH_VISION;
    } 
    else if(pitch_remote_mode) {
        // 遥控模式 - 根据遥控器输入调整目标角度
			
        float pitch_changes = ((float)rc_ctrl.rc.ch[1]) * (0.01f / 660.0f);
        pitch_target_angle += pitch_changes;
			
        // 限制目标角度范围
        if(pitch_target_angle > 0.325f) pitch_target_angle = 0.325f;
        if(pitch_target_angle < -0.325f) pitch_target_angle = -0.325f;  
    } 		
else {
        // 默认情况下保持当前位置
//        pitch_target_angle = GYRO_PITCH_ANGLE_GET_X;
    }

    // 使用位置环控制Pitch轴（仿照Yaw轴）
    PITCH_POS_PID.fdb = GYRO_PITCH_ANGLE_GET_X;
    PITCH_POS_PID.ref = pitch_target_angle;
    PID_Calc(&PITCH_POS_PID);
    float pitch_pos_output = PITCH_POS_PID.output;
    
    // 使用速度环控制Pitch轴（仿照Yaw轴）
    PITCH_SPD_PID.fdb = GYRO_G_X_XHH;
    PITCH_SPD_PID.ref = pitch_pos_output;
    PID_Calc(&PITCH_SPD_PID);
    
    // 将PID输出转换为达妙4310控制命令
    uint8_t data[8];
    
    // 位置给定（4字节浮点数，小端格式）
    float pos_set = pitch_target_angle;
    uint8_t *pos_bytes = (uint8_t*)&pos_set;
    data[0] = pos_bytes[0];
    data[1] = pos_bytes[1];
		
    data[2] = pos_bytes[2];
    data[3] = pos_bytes[3];
    
    // 速度给定（4字节浮点数，小端格式）- 使用PID输出作为速度限制
//    float vel_set = PITCH_SPD_PID.output;
		float vel_set =10;
    if(vel_set > 10.0f) vel_set = 10.0f;
    if(vel_set < -10.0f) vel_set = -10.0f;
   
    uint8_t *vel_bytes = (uint8_t*)&vel_set;
    data[4] = vel_bytes[0];
    data[5] = vel_bytes[1];
    data[6] = vel_bytes[2];
    data[7] = vel_bytes[3];
    
    // 发送到达妙4310电机（ID=1）
    CAN1_Send_Msg(0x105, data, 8);

    // 安全保护
//    if(toe_is_error(DBUS_TOE)) {
//        pid_out[0] = 0;
//        // 安全模式下停止电机
//        uint8_t stop_data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//        CAN1_Send_Msg(0x101, stop_data, 8);
//    }
    
//    // 发送yaw控制到CAN1
//    canbuf[0] = pid_out[0] >> 8;
//    canbuf[1] = pid_out[0];
//    canbuf[2] = 0;
//    canbuf[3] = 0;
//    canbuf[6] = 0;
//    canbuf[7] = 0;
//    
//    // 拨弹电机数据
//    canbuf[4] = ammunition_canbuf[4];
//    canbuf[5] = ammunition_canbuf[5];
//    
//    CAN2_Send_Msg(0x1ff, canbuf, 8);
}

float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
	/* converts unsigned int to float, given range and number of bits */
	float span = x_max - x_min;
	float offset = x_min;
	return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

void Get_Gimbal_Motor_Info(u32 motor_id, u8 *canbuf_receive)
{
	static u32 cnt[2] = {0};
	short round_cnt;
	short diff;	
	if(motor_id == 0x005) {
  // 达妙4310反馈处理 (CAN ID 0x101)
        uint8_t ID = canbuf_receive[0] & 0x0F;
        uint8_t ERR = (canbuf_receive[0] >> 4) & 0x0F;
        
        // 位置信息 (16位)
        uint16_t raw_pos = (canbuf_receive[1] << 8) | canbuf_receive[2];
        
        // 速度信息 (12位)
        uint16_t raw_vel = (canbuf_receive[3] << 4) | (canbuf_receive[4] >> 4);
        
        // 扭矩信息 (12位)
        int16_t raw_torque = ((canbuf_receive[4] & 0x0F) << 8) | canbuf_receive[5];
        
        // 温度信息
        uint8_t t_mos = canbuf_receive[6];
        uint8_t t_rotor = canbuf_receive[7];
        
        // 转换为实际值（需要根据电机参数进行校准）
        // 这里假设反馈的位置是弧度值，与陀螺仪角度单位一致
        pitch_dm4310.pos = uint_to_float(raw_pos,-12.5,12.5,16);
        pitch_dm4310.vel = uint_to_float(raw_vel,-45.0,45.0,12);
        pitch_dm4310.tor = uint_to_float(raw_torque,-18.0,18.0,12);
        
        // 更新陀螺仪角度（可选，如果电机反馈更准确）
        // GYRO_PITCH_ANGLE_GET_X = raw_pos;
//    }
}
	if(motor_id - 0x205 >= 0 && motor_id - 0x205 < 2)
	if(motor_id ==0x205)	
	{
		if(cnt[0] == 0)
		{
			gimbal_motor_info[0].last_real_angle 	= 0;		
			gimbal_motor_info[0].angle 				= -3375;		
		}
		if(cnt[1] == 0)
		{			
			gimbal_motor_info[1].last_real_angle 	= 0;						
			gimbal_motor_info[1].angle =  -666;//-2600;//GIMBAL_PITCH_OFFSET;			
		}
		
		gimbal_motor_info[motor_id - 0x205].real_angle 			= canbuf_receive[0] << 8 | canbuf_receive[1];
		gimbal_motor_info[motor_id - 0x205].raw_rate 	     	= (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		gimbal_motor_info[motor_id - 0x205].set_current 		= (signed short)( canbuf_receive[4] <<8 | canbuf_receive[5]);	
		//获取改变量
		diff = gimbal_motor_info[motor_id - 0x205].real_angle - gimbal_motor_info[motor_id - 0x205].last_real_angle;
		
		//过零点处理
		if(diff > 7650)
		{
			--round_cnt;	//逆时针
			diff = -(8192 - diff);
		}
		else if(diff < -7650)
		{
			++round_cnt;		//顺时针
			diff = (8192 + diff);
		}
		
		++cnt[0];
		++cnt[1];	
		
		//记录角度
		gimbal_motor_info[motor_id - 0x205].last_angle 			  = gimbal_motor_info[motor_id - 0x205].angle;
		gimbal_motor_info[motor_id - 0x205].last_real_angle 	= gimbal_motor_info[motor_id - 0x205].real_angle;	
		gimbal_motor_info[motor_id - 0x205].angle += diff;
		
		detect_hook(YAW_GIMBAL_MOTOR_TOE + motor_id-0x201);
	}
}