#ifndef ROBOMASTER_PROTOCOL_H
#define ROBOMASTER_PROTOCOL_H
#include "main.h"
#include "struct_typedef.h"
#include "referee_st.h"
#define HEADER_SOF 0xA5						//SOF�̶�Ϊ0xA5
#define REF_PROTOCOL_FRAME_MAX_SIZE         128

#define REF_PROTOCOL_HEADER_SIZEUI          sizeof(frame_header_structUI_t)
#define REF_PROTOCOL_HEADER_SIZE            sizeof(frame_header_struct_t)
#define REF_PROTOCOL_CMD_SIZE               2
#define REF_PROTOCOL_CRC16_SIZE             2
#define REF_HEADER_CRC_LEN                  (REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CRC16_SIZE)
#define REF_HEADER_CRC_CMDID_LEN            (REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CRC16_SIZE + sizeof(uint16_t))
#define REF_HEADER_CMDID_LEN                (REF_PROTOCOL_HEADER_SIZE + sizeof(uint16_t))

#define REF_HEADER_CRC_LENUI                (REF_PROTOCOL_HEADER_SIZEUI + REF_PROTOCOL_CRC16_SIZE)
#define REF_HEADER_CRC_CMDID_LENUI          (REF_PROTOCOL_HEADER_SIZEUI + REF_PROTOCOL_CRC16_SIZE + sizeof(uint16_t))
#define REF_HEADER_CMDID_LENUI              (REF_PROTOCOL_HEADER_SIZEUI + sizeof(uint16_t))


#pragma pack(push, 1)

//������ID˵��
typedef enum
{
    GAME_STATE_CMD_ID                    = 0x0001,    //����״̬����
    GAME_RESULT_CMD_ID                   = 0x0002,    //�����������
    GAME_ROBOT_HP_CMD_ID                 = 0x0003,		//����������Ѫ������
    DART_STATUS_ID                   	 = 0x0004,    //���ڷ���״̬�����ڷ�������
    ICRA_BUFF_ID 						 = 0x0005,    //�˹�������ս���ӳ���ͷ�״̬

    FIELD_EVENTS_CMD_ID                  = 0x0101, 		//�����¼����ݣ��¼��ı����
    SUPPLY_PROJECTILE_ACTION_CMD_ID      = 0x0102,    //���ز���վ������ʶ���ݣ������ı����
    SUPPLY_PROJECTILE_BOOKING_CMD_ID     = 0x0103,    //���󲹸�վ�������ݣ��ɲ����ӷ���
    REFEREE_WARNING_CMD_ID               = 0x0104,    //���о������ݣ����淢������
    DART_REMAINING_TIME_ID               = 0x0105,		//���ڷ���ڵ���ʱ

    ROBOT_STATE_CMD_ID                   = 0x0201,    //������״̬����
    POWER_HEAT_DATA_CMD_ID               = 0x0202,    //ʵʱ������������
    ROBOT_POS_CMD_ID                     = 0x0203,    //������λ������
    BUFF_MUSK_CMD_ID                     = 0x0204,    //��������������
    AERIAL_ROBOT_ENERGY_CMD_ID           = 0x0205,    //���л���������״̬����
    ROBOT_HURT_CMD_ID                    = 0x0206,    //�˺�״̬����
    SHOOT_DATA_CMD_ID                    = 0x0207,    //ʵʱ�������
    BULLET_REMAINING_CMD_ID              = 0x0208,    //�ӵ�ʣ�෢���������л������Լ��ڱ������˷���
    RFID_STATUS_ID                 		 = 0x0209, 		//������ RFID ״̬
    DART_CLIENT_CMD_ID   				 = 0x020A,		//���ڻ����˿ͻ���ָ����


    STUDENT_INTERACTIVE_DATA_CMD_ID      = 0x0301,    //�����˼佻�����ݣ����ͷ���������
    ROBOT_INTERACTIVE_DATA_ID			 = 0x0302,		//�Զ���������������ݽӿڣ�ͨ���ͻ��˴������ͣ����� 30Hz
    ROBOT_CMD_T_ID   					 = 0x0303,		//�ͻ���С��ͼ�������ݣ���������
    ROBOT_VERSION_ID                     = 0x0304,		//���̡������Ϣ��ͨ��ͼ�����ڷ���
    MAP_CMD_ID                           = 0x0305,		//�ͻ���С��ͼ������Ϣ

    IDCustomData,
}referee_cmd_id_t;

//֡ͷ����
typedef  struct
{
    u8 SOF;                    //��ʼ�ֽ�,�̶�0xA5
    u16 Data_Length;           //֡���ݳ���
    u8 Seq;                    //�����
    u8 CRC8;                   //CRC8У��ֵ
} frame_header_structUI_t;


typedef  struct
{
    u8 SOF;                    //��ʼ�ֽ�,�̶�0xA5
    u16 Data_Length;           //֡���ݳ���
    u8 Seq;                    //�����
    u8 CRC8;                   //CRC8У��ֵ
    u16 CMD_ID;
} frame_header_struct_t;  //ֻ��UI���õ�



//
typedef enum
{
    STEP_HEADER_SOF  = 0,
    STEP_LENGTH_LOW  = 1,
    STEP_LENGTH_HIGH = 2,
    STEP_FRAME_SEQ   = 3,
    STEP_HEADER_CRC8 = 4,
    STEP_DATA_CRC16  = 5,
} unpack_step_e;


//����ṹ�嶨��
typedef struct
{
    frame_header_structUI_t *p_header;
    uint16_t       data_len;
    uint8_t        protocol_packet[REF_PROTOCOL_FRAME_MAX_SIZE];
    unpack_step_e  unpack_step;
    uint16_t       index;
} unpack_data_t;

#pragma pack(pop)

#endif //ROBOMASTER_PROTOCOL_H
