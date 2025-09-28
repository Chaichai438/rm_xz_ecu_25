/**
  ****************************(C) COPYRIGHT SMU2021 AJI****************************
  * @file       st_UI.c/.h
  * @brief      UI??????
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Mar-18-2021     LRJ             1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT SMU2021 AJI****************************
  */


#include "st_UI.h"
#include "main.h"
#include "string.h"
#include "freertos.h"
#include "task.h"
#include "math.h"
#include "protocol.h"
#include "referee_st.h"
#include "struct_typedef.h"
#include "usart.h"
#include "stdarg.h"
#include "crc8_crc16.h"
#include "bsp_buzzer.h"
#include "referee_st.h"
#include "bsp_usart.h"
unsigned char UI_Seq;                      //?????

#define Robot_ID  UI_Data_RobotID_RStandard3			//????????????
#define Cilent_ID UI_Data_CilentID_RStandard3     //???????????

uint8_t referee_uart_tx_buf[2][150];    //????????????????DMA?????
uint8_t referee_tx_fifo = 0;            //????????????

/****************************************???????????************************************/
void UI_SendByte(u8 *ch)
{
    HAL_UART_Transmit(&huart6,ch,1, 0xffff);///<??????????????
//	buzzer_on(10, 20000);
}


/**
  * @brief          ???????
  * @param[in]      ??????????????
  * @param[in] 		??????? ??0-9
  * @retval
  */


void UI_Delete(u8 Del_Operate,u8 Del_Layer)
{

    unsigned char *framepoint;                      //??ß’???
    u16 frametail=0xFFFF;                        //CRC16ßµ???
    int i;                       //For???????????

    frame_header_struct_t framehead;
    UI_Data_Operate datahead;
    ext_client_custom_graphic_delete_t del;

    framepoint=(unsigned char *)&framehead;

    framehead.SOF=UI_SOF;
    framehead.Data_Length=8;
    framehead.Seq=UI_Seq;
    framehead.CRC8=get_CRC8_check_sum(framepoint,4,0xFF);
    framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //?????????

    datahead.Data_ID=UI_Data_ID_Del;
    datahead.Sender_ID=get_robot_id();
    datahead.Receiver_ID=get_client_id();                          //??????????

    del.operate_tpye=Del_Operate;
    del.layer=Del_Layer;                                     //???????

    frametail=get_CRC16_check_sum(framepoint,sizeof(framehead),frametail);
    framepoint=(unsigned char *)&datahead;
    frametail=get_CRC16_check_sum(framepoint,sizeof(datahead),frametail);
    framepoint=(unsigned char *)&del;
    frametail=get_CRC16_check_sum(framepoint,sizeof(del),frametail);  //CRC16ßµ???????

    framepoint=(unsigned char *)&framehead;
    for(i=0;i<sizeof(framehead);i++)
    {
        UI_SendByte(framepoint);
        framepoint++;
    }
    framepoint=(unsigned char *)&datahead;
    for(i=0;i<sizeof(datahead);i++)
    {
        UI_SendByte(framepoint);
        framepoint++;
    }
    framepoint=(unsigned char *)&del;
    for(i=0;i<sizeof(del);i++)
    {
        UI_SendByte(framepoint);
        framepoint++;
    }                                                                 //?????????
    framepoint=(unsigned char *)&frametail;
    for(i=0;i<sizeof(frametail);i++)
    {
        UI_SendByte(framepoint);
        framepoint++;                                                  //????CRC16ßµ???
    }

    UI_Seq++;                                                         //?????+1
}


/************************************************???????*************************************************
**??????*image Graph_Data???????????????????????
        imagename[3]   ?????????????????
        Graph_Operate   ??????????????
        Graph_Layer    ???0-9
        Graph_Color    ??????
        Graph_Width    ??????
        Start_x??Start_x    ???????
        End_x??End_y   ????????
**********************************************************************************************************/

void Line_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y)
{
    int i;
    for(i=0;i<3&&imagename[i]!='\0';i++)
        image->graphic_name[2-i]=imagename[i];
    image->operate_tpye = Graph_Operate;
    image->layer = Graph_Layer;
    image->color = Graph_Color;
    image->width = Graph_Width;
    image->start_x = Start_x;
    image->start_y = Start_y;
    image->end_x = End_x;
    image->end_y = End_y;
}

/************************************************???????*************************************************
**??????*image Graph_Data???????????????????????
        imagename[3]   ?????????????????
        Graph_Operate   ??????????????
        Graph_Layer    ???0-9
        Graph_Color    ??????
        Graph_Width    ??????
        Start_x??Start_x    ???????
        End_x??End_y   ?????????????????
**********************************************************************************************************/

void Rectangle_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y)
{
    int i;
    for(i=0;i<3&&imagename[i]!='\0';i++)
        image->graphic_name[2-i]=imagename[i];
    image->graphic_tpye = UI_Graph_Rectangle;
    image->operate_tpye = Graph_Operate;
    image->layer = Graph_Layer;
    image->color = Graph_Color;
    image->width = Graph_Width;
    image->start_x = Start_x;
    image->start_y = Start_y;
    image->end_x = End_x;
    image->end_y = End_y;
}

/************************************************???????*************************************************
**??????*image Graph_Data???????????????????????
        imagename[3]   ?????????????????
        Graph_Operate   ??????????????
        Graph_Layer    ???0-9
        Graph_Color    ??????
        Graph_Width    ??????
        Start_x??Start_y    ???????
        Graph_Radius  ????
**********************************************************************************************************/

void Circle_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 Graph_Radius)
{
    int i;
    for(i=0;i<3&&imagename[i]!='\0';i++)
        image->graphic_name[2-i]=imagename[i];
    image->graphic_tpye = UI_Graph_Circle;
    image->operate_tpye = Graph_Operate;
    image->layer = Graph_Layer;
    image->color = Graph_Color;
    image->width = Graph_Width;
    image->start_x = Start_x;
    image->start_y = Start_y;
    image->radius = Graph_Radius;
}

/************************************************???????*************************************************
**??????*image Graph_Data???????????????????????
        imagename[3]   ?????????????????
        Graph_Operate   ??????????????
        Graph_Layer    ???0-9
        Graph_Color    ??????
        Graph_Width    ??????
        Graph_StartAngle,Graph_EndAngle    ???????????
        Start_y,Start_y    ???????
        x_Length,y_Length   x,y???????????¶œ????
**********************************************************************************************************/

void Arc_Draw(graphic_data_struct_t *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_StartAngle,u32 Graph_EndAngle,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 x_Length,u32 y_Length)
{
    int i;

    for(i=0;i<3&&imagename[i]!='\0';i++)
        image->graphic_name[2-i]=imagename[i];
    image->graphic_tpye = UI_Graph_Arc;
    image->operate_tpye = Graph_Operate;
    image->layer = Graph_Layer;
    image->color = Graph_Color;
    image->width = Graph_Width;
    image->start_x = Start_x;
    image->start_y = Start_y;
    image->start_angle = Graph_StartAngle;
    image->end_angle = Graph_EndAngle;
    image->end_x = x_Length;
    image->end_y = y_Length;
}



/************************************************?????????????*************************************************
**??????*image Graph_Data???????????????????????
        imagename[3]   ?????????????????
        Graph_Operate   ??????????????
        Graph_Layer    ???0-9
        Graph_Color    ??????
        Graph_Width    ??????
        Graph_Size     ???
        Graph_Digit    ß≥??¶À??
        Start_x??Start_x    ???????
        Graph_Float   ?????????
**********************************************************************************************************/

void Float_Draw(Float_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Size,u32 Graph_Digit,u32 Graph_Width,u32 Start_x,u32 Start_y,float Graph_Float)
{
    int i;

    for(i=0;i<3&&imagename[i]!='\0';i++)
        image->graphic_name[2-i]=imagename[i];
    image->graphic_tpye = UI_Graph_Float;
    image->operate_tpye = Graph_Operate;
    image->layer = Graph_Layer;
    image->color = Graph_Color;
    image->width = Graph_Width;
    image->start_x = Start_x;
    image->start_y = Start_y;
    image->word_size = Graph_Size;
    image->significant_bit= Graph_Digit;
    image->graph_Float = Graph_Float;
}



/************************************************?????????????*************************************************
**??????*image Graph_Data???????????????????????
        imagename[3]   ?????????????????
        Graph_Operate   ??????????????
        Graph_Layer    ???0-9
        Graph_Color    ??????
        Graph_Size     ???
        Graph_Digit    ???????
        Graph_Width    ??????
        Start_x??Start_x    ???????
        *Char_Data          ?????????????????
**********************************************************************************************************/

void string_Draw(String_Data* image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_Size, uint32_t Graph_Digit, uint32_t Start_x, uint32_t Start_y, char* Char_Data)
{
    uint8_t i;

    for (i = 0; i < 3 && imagename[i] != '\0'; i++)
        image->Graph_Control.graphic_name[2 - i] = imagename[i];
    image->Graph_Control.graphic_tpye = UI_Graph_Char;
    image->Graph_Control.operate_tpye = Graph_Operate;
    image->Graph_Control.layer = Graph_Layer;
    image->Graph_Control.color = Graph_Color;
    image->Graph_Control.start_x = Start_x;
    image->Graph_Control.start_y = Start_y;
    image->Graph_Control.width = Graph_Size;                //???
    image->Graph_Control.start_angle = Graph_Digit;         //???

    i = 0;
    while (Char_Data[i] != '\0' && i < 30)
    {
        image->show_Data[i] = Char_Data[i];
        i++;
    }
    image->Graph_Control.end_angle = i;                     //??????????
    while (i < 30)
    {
        image->show_Data[i++] = 0;
    }

}

/************************************************UI????????????????ßπ??*********************************
**?????? cnt   ??¶»???
         ...   ??¶¡???????


Tips?????®≤??????????1??2??5??7????¶≤????????ßø??¶ƒ?ùp
**********************************************************************************************************/
int UI_ReFresh(int cnt,...)
{
    int i,n;
    graphic_data_struct_t imageData;				//
    unsigned char *framepoint;                   //??ß’???
    u16 frametail=0xFFFF;                        //CRC16ßµ???

    frame_header_struct_t framehead;				//??
    UI_Data_Operate datahead;

    va_list ap;
    va_start(ap,cnt);

    framepoint=(unsigned char *)&framehead;
    framehead.SOF=UI_SOF;
    framehead.Data_Length=6+cnt*15;
    framehead.Seq=UI_Seq;
    framehead.CRC8=get_CRC8_check_sum(framepoint,4,0xFF);
    framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //?????????

    switch(cnt)
    {
        case 1:
            datahead.Data_ID=UI_Data_ID_Draw1;
            break;
        case 2:
            datahead.Data_ID=UI_Data_ID_Draw2;
            break;
        case 5:
            datahead.Data_ID=UI_Data_ID_Draw5;
            break;
        case 7:
            datahead.Data_ID=UI_Data_ID_Draw7;
            break;
        default:
            return (-1);
    }
    datahead.Sender_ID=get_robot_id();
    datahead.Receiver_ID=get_client_id();                          //??????????

    framepoint=(unsigned char *)&framehead;
    frametail=get_CRC16_check_sum(framepoint,sizeof(framehead),frametail);
    framepoint=(unsigned char *)&datahead;
    frametail=get_CRC16_check_sum(framepoint,sizeof(datahead),frametail);          //CRC16ßµ????????????

    framepoint=(unsigned char *)&framehead;
    for(i=0;i<sizeof(framehead);i++)
    {
//	   HAL_UART_Transmit(&huart6, (u8 *)&chest,sizeof(chest), 0xffff);
        UI_SendByte(framepoint);
        framepoint++;
    }
    framepoint=(unsigned char *)&datahead;
    for(i=0;i<sizeof(datahead);i++)
    {
        UI_SendByte(framepoint);
        framepoint++;
    }

    for(i=0;i<cnt;i++)
    {
        imageData=va_arg(ap,graphic_data_struct_t);

        framepoint=(unsigned char *)&imageData;
        frametail=get_CRC16_check_sum(framepoint,sizeof(imageData),frametail);             //CRC16ßµ??

        for(n=0;n<sizeof(imageData);n++)
        {
            UI_SendByte(framepoint);
            framepoint++;
        }                                               //???????
    }
    framepoint=(unsigned char *)&frametail;
    for(i=0;i<sizeof(frametail);i++)
    {
        UI_SendByte(framepoint);
        framepoint++;                                                  //????CRC16ßµ???
    }

    va_end(ap);

    UI_Seq++;                                                         //?????+1
    return 0;
}


/************************************************UI????????????????ßπ??*********************************
**?????? cnt   ??¶»???
         ...   ??¶¡???????


Tips?????®≤??????????1??2??5??7????¶≤????????ßø??¶ƒ?ùp
**********************************************************************************************************/
int Char_ReFresh(ext_client_custom_character_t string_Data)
{
    int i;
    ext_client_custom_character_t imageData;
    unsigned char *framepoint;                      //??ß’???
    u16 frametail=0xFFFF;                        //CRC16ßµ???

    frame_header_struct_t framehead;
    UI_Data_Operate datahead;
    imageData=string_Data;


    framepoint=(unsigned char *)&framehead;
    framehead.SOF=UI_SOF;
    framehead.Data_Length=6+45;
    framehead.Seq=UI_Seq;
    framehead.CRC8=get_CRC8_check_sum(framepoint,4,0xFF);
    framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //?????????


    datahead.Data_ID=UI_Data_ID_DrawChar;

    datahead.Sender_ID=Robot_ID;
    datahead.Receiver_ID=Cilent_ID;                          //??????????

    framepoint=(unsigned char *)&framehead;
    frametail=get_CRC16_check_sum(framepoint,sizeof(framehead),frametail);
    framepoint=(unsigned char *)&datahead;
    frametail=get_CRC16_check_sum(framepoint,sizeof(datahead),frametail);
    framepoint=(unsigned char *)&imageData;
    frametail=get_CRC16_check_sum(framepoint,sizeof(imageData),frametail);             //CRC16ßµ??   //CRC16ßµ????????????

    framepoint=(unsigned char *)&framehead;
    for(i=0;i<sizeof(framehead);i++)
    {
        UI_SendByte(framepoint);
        framepoint++;
    }
    framepoint=(unsigned char *)&datahead;
    for(i=0;i<sizeof(datahead);i++)
    {
        UI_SendByte(framepoint);
        framepoint++;
    }                                                   //???????????
    framepoint=(unsigned char *)&imageData;
    for(i=0;i<sizeof(imageData);i++)
    {
        UI_SendByte(framepoint);
        framepoint++;
    }                                               //???????



    framepoint=(unsigned char *)&frametail;
    for(i=0;i<sizeof(frametail);i++)
    {
        UI_SendByte(framepoint);
        framepoint++;                                                  //????CRC16ßµ???
    }


    UI_Seq++;                                                         //?????+1
    return 0;
}

/**
 *  @brief          ????????UI?????????
 *  @author         LH.Mxy
 *  @param[in]      ???????????????
 *  @return         none
 */
void ui_display_string(String_Data* str_data)
{
    frame_header_struct_t framehead;
    UI_Data_Operate operate;

    //1. ??????????
    framehead.SOF = 0xA5;
    framehead.Data_Length = sizeof(UI_Data_Operate) + sizeof(String_Data);  //????6+45
    framehead.Seq = UI_Seq++;
    framehead.CMD_ID = 0x301;

    //2. ?????????????????
    operate.Data_ID = 0x110;                //?????ID,?????????
    operate.Sender_ID = get_robot_id();     //?????ID
    operate.Receiver_ID = get_client_id();  //?????ID

    //3. ???????
    memcpy(referee_uart_tx_buf[referee_tx_fifo], &framehead, sizeof(frame_header_struct_t));
    memcpy(referee_uart_tx_buf[referee_tx_fifo] + sizeof(frame_header_struct_t), &operate, sizeof(UI_Data_Operate));
    memcpy(referee_uart_tx_buf[referee_tx_fifo] + sizeof(frame_header_struct_t) + sizeof(UI_Data_Operate), str_data, sizeof(String_Data));

    //4. CRCßµ??
    append_CRC8_check_sum(referee_uart_tx_buf[referee_tx_fifo], 5);
    append_CRC16_check_sum(referee_uart_tx_buf[referee_tx_fifo], sizeof(frame_header_struct_t) + sizeof(UI_Data_Operate) + sizeof(String_Data) + 2);

    //5. DMA????

    usart6_tx_dma_enable(referee_uart_tx_buf[referee_tx_fifo], sizeof(frame_header_struct_t) + sizeof(UI_Data_Operate) + sizeof(String_Data) + 2);
    referee_tx_fifo = referee_tx_fifo == 0 ? 1 : 0;
}
