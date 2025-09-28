/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       remote_control.c/h
  * @brief      ң��������ң������ͨ������SBUS��Э�鴫�䣬����DMA���䷽ʽ��ԼCPU
  *             ��Դ�����ô��ڿ����ж�������������ͬʱ�ṩһЩ��������DMA������
  *             �ķ�ʽ��֤�Ȳ�ε��ȶ��ԡ�
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *  V1.0.0     Nov-11-2019     RM              1. support development board tpye c
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */
#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include "main.h"
#include "struct_typedef.h"
#include "bsp_rc.h"

#define SBUS_RX_BUF_NUM 36u

#define RC_FRAME_LENGTH 18u

#define RC_CH_VALUE_MIN         ((uint16_t)364)
#define RC_CH_VALUE_OFFSET      ((uint16_t)1024)
#define RC_CH_VALUE_MAX         ((uint16_t)1684)

/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP                ((uint16_t)1)
#define RC_SW_MID               ((uint16_t)3)
#define RC_SW_DOWN              ((uint16_t)2)
#define switch_is_down(s)       (s == RC_SW_DOWN)
#define switch_is_mid(s)        (s == RC_SW_MID)
#define switch_is_up(s)         (s == RC_SW_UP)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W            ((uint16_t)1 << 0)
#define KEY_PRESSED_OFFSET_S            ((uint16_t)1 << 1)
#define KEY_PRESSED_OFFSET_A            ((uint16_t)1 << 2)
#define KEY_PRESSED_OFFSET_D            ((uint16_t)1 << 3)
#define KEY_PRESSED_OFFSET_SHIFT        ((uint16_t)1 << 4)
#define KEY_PRESSED_OFFSET_CTRL         ((uint16_t)1 << 5)
#define KEY_PRESSED_OFFSET_Q            ((uint16_t)1 << 6)
#define KEY_PRESSED_OFFSET_E            ((uint16_t)1 << 7)
#define KEY_PRESSED_OFFSET_R            ((uint16_t)1 << 8)
#define KEY_PRESSED_OFFSET_F            ((uint16_t)1 << 9)
#define KEY_PRESSED_OFFSET_G            ((uint16_t)1 << 10)
#define KEY_PRESSED_OFFSET_Z            ((uint16_t)1 << 11)
#define KEY_PRESSED_OFFSET_X            ((uint16_t)1 << 12)
#define KEY_PRESSED_OFFSET_C            ((uint16_t)1 << 13)
#define KEY_PRESSED_OFFSET_V            ((uint16_t)1 << 14)
#define KEY_PRESSED_OFFSET_B            ((uint16_t)1 << 15)
/* ----------------------- Data Struct ------------------------------------- */
typedef __packed struct
{
        __packed struct
        {
                int16_t ch[5];
                char s[2];
        } rc;
        __packed struct
        {
                int16_t x;
                int16_t y;
                int16_t z;
                uint8_t press_l;
                uint8_t press_r;
        } mouse;
        __packed struct
        {
                uint16_t v;
        } key;

} RC_ctrl_t;

/* ----------------------- Internal Data ----------------------------------- */
//���̰�������
#define RC_ACTION_KEY_PRESS 	1
#define RC_ACTION_KEY_UNPRESS	2
//��갴������
#define RC_ACTION_MOUSE_L_PRESS			(0x01<<0)
#define RC_ACTION_MOUSE_L_UNPRESS		(0x01<<1)
#define RC_ACTION_MOUSE_R_PRESS			(0x01<<2)
#define RC_ACTION_MOUSE_R_UNPRESS		(0x01<<3)



//���̰���
enum RC_Key { 
	RC_KEY_W, 
	RC_KEY_S, 
	RC_KEY_A, 
	RC_KEY_D, 
	RC_KEY_SHIFT,  //������ת
	RC_KEY_CTRL,  
	RC_KEY_Q,      //����
	RC_KEY_E,      //����
	RC_KEY_R,      //����
	RC_KEY_F,
	RC_KEY_G,
	RC_KEY_Z,     //��ʼ��
	RC_KEY_X,			//pitch��ϵ�
	RC_KEY_C,       
	RC_KEY_V,     //
	RC_KEY_B};


//����״̬��ȡ������Ϊ1��δ��Ϊ0��
u8 RC_Key_Read( u8 rc_key);
u8 RC_Mouse_Action_Read( u8 mouse_action);
//����״̬�仯��ȡ
u8 RC_Key_Action_Read( u8 rc_key, u8 key_action);
//��ȡ����״̬�仯
void RC_Action(void);


extern void remote_control_init(void);
extern const RC_ctrl_t *get_remote_control_point(void);
extern uint8_t RC_data_is_error(void);
extern void slove_RC_lost(void);
extern void slove_data_error(void);
extern void sbus_to_usart1(uint8_t *sbus);
#endif
