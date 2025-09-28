#include "chassis.h"
#include "chassis.h"
#include "can.h"
#include "usart.h"
#include "chassis_power.h"
#include "pid_regulator.h"
#include "referee_st.h"
#include "bsp_buzzer.h"
#include "remote_control.h"
#include "motor.h"
#include "detect_task.h"
#include "chassis_ctrl.h"

volatile uint32_t chassis_offline_cnt[4] = {0};			//���̵��߼���������ʱ���ж������ӣ������ж����㣩



PID_Regulator_t CHASSIS_SPD_PID[4];
chassis_motor_back chassis_motor_info[4];
float powerdata[4];

void Chassis_pid_init(void)
{
	u8 t;
	for( t = 0; t < 4; ++t)
	{	
		CHASSIS_SPD_PID[t].kp=4.0f;
		CHASSIS_SPD_PID[t].ki=0.0f;
		CHASSIS_SPD_PID[t].kd=0.0f;
		CHASSIS_SPD_PID[t].componentKpMax=10000;
		CHASSIS_SPD_PID[t].componentKiMax=3000;
		CHASSIS_SPD_PID[t].componentKdMax=0;
		CHASSIS_SPD_PID[t].outputMax=13000;
		
		chassis_motor_info[t].angle 	= 0;
		chassis_motor_info[t].angle_set = 0;
	}	
}

float chassis_spd[4];
float power_residuals;
float Watch_Power_Max;
float Klimit=1;
float Plimit=0;
float Watch_Power;
float Watch_Buffer;
static uint16_t  Power_Max;
float Power,Power_Buffer;
float Chassis_pidout_max;
//double Scaling1=0,Scaling2=0,Scaling3=0,Scaling4=0;
static double Scaling1,Scaling2,Scaling3,Scaling4;
float Chassis_pidout_max;
double Chassis_pidout;

//�����ĸ����̵�����ٶ�
void Chassis_Power_Limit(void)   //���̹�������
{	
	//819.2/A
	Watch_Power_Max=Klimit;	Watch_Power=Power;	Watch_Buffer=Power_Buffer;
	get_chassis_power_and_buffer(&Power, &Power_Buffer);

		Chassis_pidout_max=1536;//32768��40��960
			PID_Calc(&CHASSIS_SPD_PID[0]);
		 PID_Calc(&CHASSIS_SPD_PID[1]);
		 PID_Calc(&CHASSIS_SPD_PID[2]);
		 PID_Calc(&CHASSIS_SPD_PID[3]);

//	if(Power>960)	Chassis_VAL_LIMIT(4096);//5*4*24;
//	else
//		{
//	else if(Power_Buffer<=55){
		Chassis_pidout=(
						ABS(CHASSIS_SPD_PID[0].ref-CHASSIS_SPD_PID[0].fdb)+
						ABS(CHASSIS_SPD_PID[1].ref-CHASSIS_SPD_PID[1].fdb)+
						ABS(CHASSIS_SPD_PID[2].ref-CHASSIS_SPD_PID[2].fdb)+
						ABS(CHASSIS_SPD_PID[3].ref-CHASSIS_SPD_PID[3].fdb));
	
		Scaling1=(CHASSIS_SPD_PID[0].ref-CHASSIS_SPD_PID[0].fdb)/Chassis_pidout;	
		Scaling2=(CHASSIS_SPD_PID[1].ref-CHASSIS_SPD_PID[1].fdb)/Chassis_pidout;
		Scaling3=(CHASSIS_SPD_PID[2].ref-CHASSIS_SPD_PID[2].fdb)/Chassis_pidout;	
		Scaling4=(CHASSIS_SPD_PID[3].ref-CHASSIS_SPD_PID[3].fdb)/Chassis_pidout;
		Klimit=Chassis_pidout/1500;
	//	VAL_LIMIT(Klimit,-1,1);

//		if(Power_Buffer<50&&Power_Buffer>=40)	Plimit=0.6;//15
//		else if(Power_Buffer<40&&Power_Buffer>=35)	Plimit=0.5;
//		else if(Power_Buffer<35&&Power_Buffer>=30)	Plimit=0.35;
//		else if(Power_Buffer<30&&Power_Buffer>=20)	Plimit=0.25;
//		else if(Power_Buffer<20&&Power_Buffer>=10)	Plimit=0.125;
//		else if(Power_Buffer<10&&Power_Buffer>=0)	Plimit=0.05;
//		else if(Power_Buffer==60)					Plimit=1;
		if(Power_Buffer<50&&Power_Buffer>=40)	Plimit=0.8;//15
		else if(Power_Buffer<40&&Power_Buffer>=35)	Plimit=0.6;
		else if(Power_Buffer<35&&Power_Buffer>=30)	Plimit=0.45;
		else if(Power_Buffer<30&&Power_Buffer>=20)	Plimit=0.25;
		else if(Power_Buffer<20&&Power_Buffer>=10)	Plimit=0.125;
		else if(Power_Buffer<10&&Power_Buffer>=0)	Plimit=0.05;
		else if(Power_Buffer==60)					Plimit=1;
		 CHASSIS_SPD_PID[0].output=Scaling1*Chassis_pidout_max*Klimit*Plimit;
	   CHASSIS_SPD_PID[1].output=Scaling2*Chassis_pidout_max*Klimit*Plimit;
     CHASSIS_SPD_PID[2].output=Scaling3*Chassis_pidout_max*Klimit*Plimit;
     CHASSIS_SPD_PID[3].output=Scaling4*Chassis_pidout_max*Klimit*Plimit;/*ͬ�����ŵ���*/

//		//PID_Chassis_Motor4_Speed.out = PID_Chassis_Motor1_Speed.out*(float)(Power_Buffer+20)/Power_Max*Chassis_pidout_max;//ͬ������������ֵ
//		}	
}
void Chassis_Motor_Speed_Set(float vx,float vy,float vyaw)
{
	short pid_out[4];
	u8 t;
	u8 tt;
	u8 canbuf[8];
	
		power_residuals=game_info.game_robot_state.chassis_power_limit-game_info.power_heat_data.chassis_power;//ʣ�๦��
	chassis_spd[3] =   vy - vx + vyaw * ( CHASSIS_A + CHASSIS_B);			//��ǰ
	chassis_spd[2] =   vy + vx + vyaw * ( CHASSIS_A + CHASSIS_B);			//��ǰ
	chassis_spd[1] = -(vy - vx - vyaw * ( CHASSIS_A + CHASSIS_B));			//���
	chassis_spd[0] = -(vy + vx - vyaw * ( CHASSIS_A + CHASSIS_B));			//�Һ�

		
	//������Ϣ
	for( t = 0; t < 4; ++t)
	{
		chassis_spd[t] = chassis_spd[t] * 2 * 19 * 60;			//�ٶ�ת��Ϊת��
	}
			for( t = 0; t < 4; ++t){
//				if(chassis_offline_cnt[t] < 100){		//����ʱ��С��100ms�������㣬���򲻿��Ƹõ��
					CHASSIS_SPD_PID[t].fdb=chassis_motor_info[t].rpm;
					CHASSIS_SPD_PID[t].ref=chassis_spd[t];
					PID_Calc(&CHASSIS_SPD_PID[t]);
//					Chassis_Power_Limit();
					pid_out[t] = CHASSIS_SPD_PID[t].output;		
					chassis_motor_info[t].angle_set = chassis_motor_info[t].angle;
				
//				}			
		}
			
//						
//	if(chassis_spd[0]==0&&chassis_spd[1]==0&&chassis_spd[2]==0&&chassis_spd[3]==0)//���¿����Լ�����
//	{
//		if(power_residuals<10)
//			{
//		for( tt = 0; tt < 		4; ++tt)
//		{
//			pid_out[tt]=0;
//		}
//			}
//	}
	if(toe_is_error(DBUS_TOE))
	{
		pid_out[0]=0;
		pid_out[1]=0;
		pid_out[2]=0;
		pid_out[3]=0;
	}
	canbuf[0] = pid_out[0] >> 8;
	canbuf[1] = pid_out[0];
	canbuf[2] = pid_out[1] >> 8;
	canbuf[3] = pid_out[1];
	canbuf[4] = pid_out[2] >> 8;
	canbuf[5] = pid_out[2];
	canbuf[6] = pid_out[3] >> 8;
	canbuf[7] = pid_out[3];

	CAN2_Send_Msg( 0x200, canbuf, 8);		//��������

}





void Get_Chassis_Motor_Info( uint32_t motor_id, u8 *canbuf_receive)
{
	static u32 cnt = 0;
	short diff;
	
	if( (motor_id - 0x201 >= 0) && (motor_id - 0x201 < 4))
	{
//		chassis_offline_cnt[motor_id - 0x201] = 0;		//������߼���������
		
		chassis_motor_info[motor_id - 0x201].last_real_angle = chassis_motor_info[motor_id - 0x201].real_angle;			//�����ϴνǶ�
		
		//��ȡ��ǰ��Ϣ
		chassis_motor_info[motor_id - 0x201].real_angle 	= canbuf_receive[0] << 8 | canbuf_receive[1];
		chassis_motor_info[motor_id - 0x201].rpm 			= (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		chassis_motor_info[motor_id - 0x201].real_current 	= (signed short)(canbuf_receive[4] << 8 | canbuf_receive[5]);
		chassis_motor_info[motor_id - 0x201].temperature 	= canbuf_receive[6];
		
		chassis_motor_info[motor_id - 0x201].current 		= (float)chassis_motor_info[motor_id - 0x201].real_current / 819.2f;
		
		
		//����㴦��
		diff = chassis_motor_info[motor_id - 0x201].real_angle - chassis_motor_info[motor_id - 0x201].last_real_angle;
		if( diff > 7500)
		{
			diff = -(8192 - diff);
		}
		else if( diff < -7500)
		{
			diff = (8192 + diff);
		}
		
		
		//�ų���һ�ε�����	
		if( cnt == 0)
			diff = 0;				
		++cnt;

		
		//�ǶȻ���
		chassis_motor_info[motor_id - 0x201].angle += diff;
		detect_hook(CHASSIS_MOTOR1_TOE + motor_id-0x201);
	}
	
}

void pid_pro()
{
	fp32 error1,error2;
	u8 t;
	error1 = chassis_motor_info[t].angle_set-chassis_motor_info[t].angle;
	error2 = chassis_motor_info[t].angle -chassis_motor_info[t].last_real_angle;
	if(error1 >500 && error2<20)
	{
		chassis_motor_info[t].angle_set =chassis_motor_info[t].angle;
	}
}
void Get_Chaojidianrong(uint32_t motor_id, u8 *canbuf_receive)
{
	if(motor_id == 0x211)
	{
		
		powerdata[0]= (canbuf_receive[1]<<8|canbuf_receive[0])/100.0f;//�����ѹ
		powerdata[1]=(canbuf_receive[2]| canbuf_receive[3]<<8)/100.0f ;//���ݵ�ѹ
		powerdata[2]=(canbuf_receive[4] | canbuf_receive[5]<<8)/100.0f ;//�������
		powerdata[3]=(canbuf_receive[6] | canbuf_receive[7]<<8)/100.0f ;//�趨����
	}
}