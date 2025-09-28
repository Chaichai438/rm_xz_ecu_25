#include "chassis_power.h"
//#include "judge_19_data.h"
#include "chassis.h"
#include "usart.h"
 int temp_v,temp_i;
  float dianya;
float chassis_pmax(void)
{
//	if(game_info.power_heat_data.chassisPowerBuffer>WD)
//		return(((game_info.power_heat_data.chassisPowerBuffer-WD)/T_POWER)+80);
//	else
		return 80.0f;
}

void chassis_power(short *motor_current)
{
	short i;
	float pmax,current_c,voltage_c,imax,k;
//	current_c=(float)game_info.power_heat_data.chassis_current/1000;
//	voltage_c=(float)game_info.power_heat_data.chassis_volt/1000;
	imax=pmax/voltage_c;
	//printf("v=%f\r\n",voltage_c);
	//printf("c=%f\r\n",current_c);
	//printf("p=%f\r\n",voltage_c*current_c);
	pmax=80.0;
	imax=pmax/voltage_c;
	k=imax/current_c;
	//printf("c_s:%d\r\n",current_sum);
	
	if(current_c< imax)
		return;
	else
	{
		//printf("jingru\r\n");
		for(i=0;i<4;i++)
		{
			motor_current[i]=(short)(k*motor_current[i]);
		}
	}
}
void Get_Power_Info(uint32_t id, u8 *canbuf_receive)
{
	//float power_v,power_i;
	u16 t[4];
//			printf("123\r\n");
//	printf("id%d\r\n",id);
//		printf("i=%fA\r\n",temp_i/1000.0);
	if(id==0x211)
	{
		t[0]=canbuf_receive[1] << 8 | canbuf_receive[0];
		t[1]=canbuf_receive[3] << 8 | canbuf_receive[2];      
		t[2]=canbuf_receive[5] << 8 | canbuf_receive[4];
		t[3]=canbuf_receive[7] << 8 | canbuf_receive[8];
		temp_v=t[0] << 16 | t[1];
		temp_i=t[2] << 16 | t[3];
		dianya=t[1]/100.0f;
//		printf("qwe\r\n");
//		printf("v=%fV\r\n",temp_v/1000.0);
//		printf("i=%fA\r\n",temp_i/1000.0);
	}
}
// void test ()
// {
// 
// 
// printf("v=%fV\r\n",temp_v/1000.0);
// printf("i=%fA\r\n",temp_i/1000.0);
// }
