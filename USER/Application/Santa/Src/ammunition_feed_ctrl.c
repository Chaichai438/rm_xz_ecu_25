#include "ammunition_feed_ctrl.h"
#include "main.h"
#include "ammunition_feed.h"
#include "remote_control.h"
#include "referee_st.h"
#include "cmsis_os.h"
#include "referee_usart_task_st.h"
#include "usart.h"
#include "bsp_fric.h"
#include "user_lib.h"
#include "voltage_task.h"
#include "init.h"
#include "bsp_laser.h"
#include "shoot_fric.h"

extern volatile int shoot_num;
extern RC_ctrl_t rc_ctrl;
extern ramp_function_source_t fric1_ramp,fric2_ramp;        //射击数据
extern uint16_t fric_pwm1,fric_pwm2;
int danda;

//拨弹初始化

extern Game_info game_info;
void Ammunition_Init(void)
{
	Ammunition_pid_init();
}

int now_power,BUTTEN_TRIG_PIN=0;	
uint32_t jam_cnt = 0;		//卡弹计时
uint32_t jam_t = 0;			//卡弹，电机反转计时
uint32_t con_s=0;
int inforbuff=0;
uint32_t single=0,shoot=0;
uint8_t sheng_dan;
u8 bomm_judge;

void Ammunition_Ctrl(void)
{
		BUTTEN_TRIG_PIN =HAL_GPIO_ReadPin(BUTTON_TRIG_GPIO_Port, BUTTON_TRIG_Pin);
		Shoot_fric_Ctrl();
	danda=rc_ctrl.rc.ch[4];
	
		//if(RC_Key_Read( RC_KEY_Q) == 1 ){single=1;inforbuff=1;}//单发，RC_Key_Read( RC_KEY_Q) == 1  danda>550&&rc_ctrl.rc.s[0] == 3
		if(RC_Key_Read( RC_KEY_E) == 1){single=1;inforbuff=0;shoot=0;}
		if(single ==0){normal_shoot();}
		if(single ==1&&shoot==0){single_shoot();}
		
		if(RC_Key_Read(RC_KEY_C) == 1){shoot=1;single=0;}
//		if(shoot==1||rc_ctrl.rc.s[0] == 1&&single==0)
//	      shoot=1;
		
		
		
		
		//	fire_shoot();
		
		
		
		
//if(rc_ctrl.rc.s[0] != 1)fire_shoot();
	//	if(RC_Key_Read(RC_KEY_Z) == 1)RM_init ();
		
}


void fire_shoot()
{
//	now_power=100;
	if(rc_ctrl.rc.s[1] == 3)
	{	
		if( rc_ctrl.mouse.press_l == 1)//单发与连发的区别为拨杆 rc_ctrl.rc.s[0] == 1 ||
	 {
		 if(now_power>=10)
			yaobai(3000);
		 if(now_power<10)
	 Ammunition_Motor_Speed_Set(0);
	 }
	
	 if(rc_ctrl.mouse.press_l == 0)
		Ammunition_Motor_Speed_Set(0);
 }
	if(rc_ctrl.rc.s[1] == 2||rc_ctrl.rc.s[1] == 1)
	{
		if(rc_ctrl.rc.s[0]==1)
		{
				if(now_power>=10)
				yaobai(3000);
				if(now_power<10)
				Ammunition_Motor_Speed_Set(0);
	 }
		 if(rc_ctrl.rc.s[0] == 3||rc_ctrl.rc.s[0] == 2)
			  Ammunition_Motor_Speed_Set(0);
	 }
	 
}

int t=0;
void normal_shoot()
{
	uint8_t shoot_num;
	now_power=game_info.game_robot_state.shooter_barrel_heat_limit - game_info.power_heat_data.shooter_id1_17mm_cooling_heat;//计算现有热量
	shoot_num=game_info.bullet_remaining_t.bullet_remaining_num_17mm;            //实时更新现有子弹数
	sheng_dan=now_power/10;                  //计算还可以打几个弹
	
	

	if//(rc_ctrl.mouse.press_l == 1)
		((rc_ctrl.rc.s[0] == 1 || rc_ctrl.mouse.press_l == 1) )//遥控器拨盘控制pitch轴电机开断
	 {
//	 if(sheng_dan>=3&&now_power>=15)
		yaobai(-3000);
//	 else
//		 Ammunition_Motor_Speed_Set(0);
	 }
	else Ammunition_Motor_Speed_Set(0);
	
//	if(now_power <=15)
//	{
//		if((rc_ctrl.rc.s[0] == 1 || rc_ctrl.mouse.press_l == 1) )
//		{
//			
//			if(sheng_dan >= 1&&now_power<=15)
//				yaobai(2000);
//			else
//			{
//				Ammunition_Motor_Speed_Set(-4000);
//			}
//		}
//	}
	
	
	
	
//	  正常播弹
// if(now_power<=15)
//  {
//		Ammunition_Motor_Speed_Set(0);
//	}
//else
	//{
	//判断是否拨弹（遥控器控制，热量限制）
//	if((rc_ctrl.rc.s[0] == 1 || rc_ctrl.mouse.press_l == 1) )
//	 {
//		 //if(sheng_dan >= 1&&now_power<=15)
//			yaobai(2000);
//	 }
//	else Ammunition_Motor_Speed_Set(0);
	//}
}
void single_shoot()							//单发
{
			if(rc_ctrl.mouse.press_l == 1)//鼠标的按下
		{
			if(BUTTEN_TRIG_PIN==1)//微动开关
			{
					Ammunition_Motor_Speed_Set(-3000);
			}
			if(BUTTEN_TRIG_PIN==0)
			{
					Ammunition_Motor_Speed_Set(0);
			}
		}
		if(rc_ctrl.mouse.press_l != 1)//枪管没弹补单
			
		{
					if(BUTTEN_TRIG_PIN==0)
					{
						yaobai(3000);
					}	
					if(BUTTEN_TRIG_PIN==1)
						Ammunition_Motor_Speed_Set(0);	
		}
}

void yaobai( short speed_2006)//卡弹处理函数
{
		 if(jam_t>0)//卡单反转计时大于0进行反转
	{
			Ammunition_Motor_Speed_Set(speed_2006);
			jam_t--;//反转时间自减
			con_s++;																				
			if(ammunition_motor_info.rpm <10 && con_s >27 )   
			{
					Ammunition_Motor_Speed_Set(-2100);  //-speed_2006
			}																										
	}
	else 
	{
		if(ABS(ammunition_motor_info.rpm) < 20 )
		{
			jam_cnt++;	//卡单计时
			jam_t  = jam_cnt  >25 ? 27:0 ;//进行计算比较决断是否卡单纪实
		}
		else
		{   //正常拨弹		
			jam_cnt=0;
			con_s=0;							
			Ammunition_Motor_Speed_Set(-speed_2006);
		}
	}	
}
