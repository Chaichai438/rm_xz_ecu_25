#include "Motor_ALL.h"

chassis_motor_back Motor_Whell_Info[4] = {0};

Raw_Motor_Back Motor_Fric_Info[2]={0};

gimbal_motor_back Motor_Feed_Info={0};

gimbal_motor_back Motor_Pitch_Info={0};

gimbal_motor_back Motor_Yaw_Info={0};

updown_chassisSet UDC_Set={0};


/**
  * @brief          Yawcan閹恒儱褰堥崙鑺ユ殶
  * @param     		Struct閿涙imbal_Motor_Back
  * @param[in]      motor_id: YawID
  * @param[in]      canbuf_receive: can閹恒儱褰堥弫鐗堝祦閹稿洭鎷�
  * @retval         none
  */
void Get_Gimbal_Yaw_Motor_Info( uint32_t motor_id, uint8_t *canbuf_receive)
{
	static uint32_t cnt = 0;
	short round_cnt;
	short diff;	

	if( motor_id-YAW_ID == 0 )
	{
		if( cnt == 0)
		{
			Motor_Yaw_Info.last_real_angle 	= 0;		
			Motor_Yaw_Info.vis_angle 				= YAW_ANGLE_OFFSET;
				cnt++;
		}

		Motor_Yaw_Info.real_angle 			= canbuf_receive[0] << 8 | canbuf_receive[1];
		Motor_Yaw_Info.rpm 	     	        = (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		Motor_Yaw_Info.real_current 		    = (signed short)( canbuf_receive[4] <<8 | canbuf_receive[5]);	
		//閼惧嘲褰囬弨鐟板綁闁诧拷
		diff = Motor_Yaw_Info.real_angle - Motor_Yaw_Info.last_real_angle;
		//鏉╁洭娴傞悙鐟邦槱閻烇拷
		if( diff > 7650)
		{
			--round_cnt;	//闁棙妞傞柦锟�
			diff -= 8192;
		}
		else if( diff < -7650)
		{
			++round_cnt;		//妞ょ儤妞傞柦锟�
			diff += 8192;
		}
		++cnt;	
		//鐠佹澘缍嶇憴鎺戝
		Motor_Yaw_Info.last_vis_angle 			  = Motor_Yaw_Info.vis_angle;
		Motor_Yaw_Info.last_real_angle 	= Motor_Yaw_Info.real_angle;	
		Motor_Yaw_Info.vis_angle += diff;
		if(Motor_Yaw_Info.vis_angle>8191)
			Motor_Yaw_Info.vis_angle-=8192;
		else if(Motor_Yaw_Info.vis_angle<-8191)
			Motor_Yaw_Info.vis_angle+=8192;
	}
}
/**
  * @brief          Pitchcan閹恒儱褰堥崙鑺ユ殶
  * @param     		Struct閿涙imbal_Motor_Back
  * @param[in]      motor_id: PitchID
  * @param[in]      canbuf_receive: can閹恒儱褰堥弫鐗堝祦閹稿洭鎷�
  * @retval         none
  */
void Get_Gimbal_Pitch_Motor_Info( uint32_t motor_id, uint8_t *canbuf_receive)
{
//	static uint32_t cnt = 0;
	short round_cnt;
	short diff;	

	if( motor_id-Chassis3_ID == 0 )
	{
//		if( cnt == 0)
//		{
//			Motor_Pitch_Info.last_real_angle 	= 0;		
//			Motor_Pitch_Info.vis_angle 				= YAW_ANGLE_OFFSET;		
//		}

		Motor_Pitch_Info.real_angle 			= canbuf_receive[0] << 8 | canbuf_receive[1];
		Motor_Pitch_Info.rpm 	     	        = (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		Motor_Pitch_Info.real_current 		    = (signed short)( canbuf_receive[4] <<8 | canbuf_receive[5]);	
		//閼惧嘲褰囬弨鐟板綁闁诧拷
		diff = Motor_Pitch_Info.real_angle - Motor_Pitch_Info.last_real_angle;
		//鏉╁洭娴傞悙鐟邦槱閻烇拷
		if( diff > 7650)
		{
			--round_cnt;	//闁棙妞傞柦锟�
			diff -= 8192;
		}
		else if( diff < -7650)
		{
			++round_cnt;		//妞ょ儤妞傞柦锟�
			diff += 8192;
		}
//		++cnt;	
		//鐠佹澘缍嶇憴鎺戝
		Motor_Pitch_Info.last_vis_angle 			  = Motor_Pitch_Info.vis_angle;
		Motor_Pitch_Info.last_real_angle 	= Motor_Pitch_Info.real_angle;	
		Motor_Pitch_Info.vis_angle += diff;
	}
}
/**
  * @brief          閹枫劌鑴奵an閹恒儱褰堥崙鑺ユ殶   
  * @param     		Struct閿涙imbal_Motor_Back
  * @param[in]      motor_id: 閹枫劌鑴奍D
  * @param[in]      canbuf_receive: can閹恒儱褰堥弫鐗堝祦閹稿洭鎷�
  * @retval         none
  */
void Get_Feed_Motor_Info( uint32_t motor_id, uint8_t *canbuf_receive)
{
	static uint32_t cnt = 0;
	short round_cnt;
	short diff;	

	if( motor_id-FEED_ID == 0 )
	{
//		if( cnt == 0)
//		{
//			Motor_Feed_Info.last_real_angle 	= 0;		
//			Motor_Feed_Info.vis_angle 				= YAW_ANGLE_OFFSET;		
//		}

		Motor_Feed_Info.real_angle 			= canbuf_receive[0] << 8 | canbuf_receive[1];
		Motor_Feed_Info.rpm 	     	        = (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		Motor_Feed_Info.real_current 		    = (signed short)( canbuf_receive[4] <<8 | canbuf_receive[5]);	
		//閼惧嘲褰囬弨鐟板綁闁诧拷
		diff = Motor_Feed_Info.real_angle - Motor_Feed_Info.last_real_angle;
		//鏉╁洭娴傞悙鐟邦槱閻烇拷
		if( diff > 7650)
		{
			--round_cnt;	//闁棙妞傞柦锟�
			diff -= 8192;
		}
		else if( diff < -7650)
		{
			++round_cnt;		//妞ょ儤妞傞柦锟�
			diff += 8192;
		}
		++cnt;	
		//鐠佹澘缍嶇憴鎺戝
		Motor_Feed_Info.last_vis_angle 			  = Motor_Feed_Info.vis_angle;
		Motor_Feed_Info.last_real_angle 	= Motor_Feed_Info.real_angle;	
		Motor_Feed_Info.vis_angle += diff;
	}
}
/**
  * @brief          閹解晜鎽濇潪鐢n閹恒儱褰堥崙鑺ユ殶
  * @param     		Struct閿涙瓓aw_Motor_Back
  * @param[in]      motor_id: 閹解晜鎽濇潪鐢€D
  * @param[in]      canbuf_receive: can閹恒儱褰堥弫鐗堝祦閹稿洭鎷�
  * @retval         none
  */
void Get_Fric_Motor_Info( uint32_t motor_id, uint8_t *canbuf_receive)
{

	if( motor_id-Fric0_ID == 0 )
	{
		Motor_Fric_Info[0].real_angle 			= canbuf_receive[0] << 8 | canbuf_receive[1];
		Motor_Fric_Info[0].rpm 	     	        = (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		Motor_Fric_Info[0].real_current 		    = (signed short)( canbuf_receive[4] <<8 | canbuf_receive[5]);	
	}else if( motor_id-Fric1_ID == 0 )
	{
		Motor_Fric_Info[1].real_angle 			= canbuf_receive[0] << 8 | canbuf_receive[1];
		Motor_Fric_Info[1].rpm 	     	        = (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		Motor_Fric_Info[1].real_current 		    = (signed short)( canbuf_receive[4] <<8 | canbuf_receive[5]);	
	}
}

/**
  * @brief          閹解晜鎽濇潪鐢n閹恒儱褰堥崙鑺ユ殶
  * @param     		Struct閿涙瓓aw_Motor_Back
  * @param[in]      motor_id: 閹解晜鎽濇潪鐢€D
  * @param[in]      canbuf_receive: can閹恒儱褰堥弫鐗堝祦閹稿洭鎷�
  * @retval         none
  */
void Get_Chassis_Motor_Info( uint32_t motor_id, uint8_t *canbuf_receive)
{

	if( motor_id - Chassis1_ID >=0 && motor_id - Chassis1_ID < 4 )
	{
		Motor_Whell_Info[motor_id - Chassis1_ID].real_angle 			= canbuf_receive[0] << 8 | canbuf_receive[1];
		Motor_Whell_Info[motor_id - Chassis1_ID].rpm 	     	        = (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		Motor_Whell_Info[motor_id - Chassis1_ID].real_current 		    = (signed short)( canbuf_receive[4] <<8 | canbuf_receive[5]);	
	}

}

void Get_UP_Board_Chassis_Info( uint32_t motor_id, uint8_t *canbuf_receive)
{
	
	if( motor_id-0x168/*UP_DOWNID*/ == 0 )
	{
	UDC_Set.motor0set = canbuf_receive[0] << 8 | canbuf_receive[1];
	UDC_Set.motor1set = canbuf_receive[2] << 8 | canbuf_receive[3];
	UDC_Set.motor2set = canbuf_receive[4] << 8 | canbuf_receive[5];
	UDC_Set.motor3set = canbuf_receive[6] << 8 | canbuf_receive[7];
	}
	
}
