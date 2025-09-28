/**
  ****************************(C) COPYRIGHT SMU2021 AJI****************************
  * @file       st_UI.c/.h
  * @brief      UI相关操作
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
unsigned char UI_Seq;                      //包序号

#define Robot_ID  UI_Data_RobotID_RStandard3			//机器人角色设置
#define Cilent_ID UI_Data_CilentID_RStandard3     //客户端角色设置

uint8_t referee_uart_tx_buf[2][150];    //裁判系统学生串口发送DMA缓冲池
uint8_t referee_tx_fifo = 0;            //正在使用的缓存池

/****************************************串口驱动映射************************************/
void UI_SendByte(u8 *ch)
{
   HAL_UART_Transmit(&huart6,ch,1, 0xffff);///<普通串口发送数据
//	buzzer_on(10, 20000);
}


/**
  * @brief          删除操作
  * @param[in]      对应头文件删除操作
  * @param[in] 		要删除的层 取值0-9
  * @retval         
  */


void UI_Delete(u8 Del_Operate,u8 Del_Layer)
{

   unsigned char *framepoint;                      //读写指针
   u16 frametail=0xFFFF;                        //CRC16校验值
   int i;                       //For函数循环控制
   
   frame_header_struct_t framehead;
   UI_Data_Operate datahead;
   ext_client_custom_graphic_delete_t del;
   
   framepoint=(unsigned char *)&framehead;
   
   framehead.SOF=UI_SOF;
   framehead.Data_Length=8;
   framehead.Seq=UI_Seq;
   framehead.CRC8=get_CRC8_check_sum(framepoint,4,0xFF);
   framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //填充包头数据
   
   datahead.Data_ID=UI_Data_ID_Del;
   datahead.Sender_ID=get_robot_id();
   datahead.Receiver_ID=get_client_id();                          //填充操作数据
   
   del.operate_tpye=Del_Operate;
   del.layer=Del_Layer;                                     //控制信息
   
   frametail=get_CRC16_check_sum(framepoint,sizeof(framehead),frametail);
   framepoint=(unsigned char *)&datahead;
   frametail=get_CRC16_check_sum(framepoint,sizeof(datahead),frametail);
   framepoint=(unsigned char *)&del;
   frametail=get_CRC16_check_sum(framepoint,sizeof(del),frametail);  //CRC16校验值计算
   
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
   }                                                                 //发送所有帧
   framepoint=(unsigned char *)&frametail;
   for(i=0;i<sizeof(frametail);i++)
   {
      UI_SendByte(framepoint);
      framepoint++;                                                  //发送CRC16校验值
   }
   
   UI_Seq++;                                                         //包序号+1
}


/************************************************绘制直线*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Start_x、Start_x    开始坐标
        End_x、End_y   结束坐标
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

/************************************************绘制矩形*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Start_x、Start_x    开始坐标
        End_x、End_y   结束坐标（对顶角坐标）
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

/************************************************绘制整圆*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Start_x、Start_y    圆心坐标
        Graph_Radius  图形半径
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

/************************************************绘制圆弧*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Graph_StartAngle,Graph_EndAngle    开始，终止角度
        Start_y,Start_y    圆心坐标
        x_Length,y_Length   x,y方向上轴长，参考椭圆
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



/************************************************绘制浮点型数据*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Graph_Size     字号
        Graph_Digit    小数位数
        Start_x、Start_x    开始坐标
        Graph_Float   要显示的变量
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



/************************************************绘制字符型数据*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Size     字号
        Graph_Digit    字符个数
        Graph_Width    图形线宽
        Start_x、Start_x    开始坐标
        *Char_Data          待发送字符串开始地址
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
    image->Graph_Control.width = Graph_Size;                //线宽
    image->Graph_Control.start_angle = Graph_Digit;         //字号

    i = 0;
    while (Char_Data[i] != '\0' && i < 30)
    {
        image->show_Data[i] = Char_Data[i];
        i++;
    }
    image->Graph_Control.end_angle = i;                     //记录字符长度
    while (i < 30)
    {
        image->show_Data[i++] = 0;
    }
    
}

/************************************************UI推送函数（使更改生效）*********************************
**参数： cnt   图形个数
         ...   图形变量参数


Tips：：该函数只能推送1，2，5，7个图形，其他数目协议未涉及
**********************************************************************************************************/
int UI_ReFresh(int cnt,...)
{
   int i,n;
   graphic_data_struct_t imageData;				//
   unsigned char *framepoint;                   //读写指针
   u16 frametail=0xFFFF;                        //CRC16校验值
   
   frame_header_struct_t framehead;				//帧头
   UI_Data_Operate datahead;					
   
   va_list ap;
   va_start(ap,cnt);
   
   framepoint=(unsigned char *)&framehead;
   framehead.SOF=UI_SOF;
   framehead.Data_Length=6+cnt*15;
   framehead.Seq=UI_Seq;
   framehead.CRC8=get_CRC8_check_sum(framepoint,4,0xFF);
   framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //填充包头数据
   
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
   datahead.Receiver_ID=get_client_id();                          //填充操作数据
   
   framepoint=(unsigned char *)&framehead;
   frametail=get_CRC16_check_sum(framepoint,sizeof(framehead),frametail);
   framepoint=(unsigned char *)&datahead;
   frametail=get_CRC16_check_sum(framepoint,sizeof(datahead),frametail);          //CRC16校验值计算（部分）
   
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
      frametail=get_CRC16_check_sum(framepoint,sizeof(imageData),frametail);             //CRC16校验
      
      for(n=0;n<sizeof(imageData);n++)
      {
         UI_SendByte(framepoint);
         framepoint++;             
      }                                               //发送图片帧
   }
   framepoint=(unsigned char *)&frametail;
   for(i=0;i<sizeof(frametail);i++)
   {
      UI_SendByte(framepoint);
      framepoint++;                                                  //发送CRC16校验值
   }
   
   va_end(ap);
   
   UI_Seq++;                                                         //包序号+1
   return 0;
}


/************************************************UI推送字符（使更改生效）*********************************
**参数： cnt   图形个数
         ...   图形变量参数


Tips：：该函数只能推送1，2，5，7个图形，其他数目协议未涉及
**********************************************************************************************************/
int Char_ReFresh(ext_client_custom_character_t string_Data)
{
   int i;
   ext_client_custom_character_t imageData;
   unsigned char *framepoint;                      //读写指针
   u16 frametail=0xFFFF;                        //CRC16校验值
   
   frame_header_struct_t framehead;
   UI_Data_Operate datahead;
   imageData=string_Data;
   
   
   framepoint=(unsigned char *)&framehead;
   framehead.SOF=UI_SOF;
   framehead.Data_Length=6+45;
   framehead.Seq=UI_Seq;
   framehead.CRC8=get_CRC8_check_sum(framepoint,4,0xFF);
   framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //填充包头数据
   

   datahead.Data_ID=UI_Data_ID_DrawChar;

   datahead.Sender_ID=Robot_ID;
   datahead.Receiver_ID=Cilent_ID;                          //填充操作数据
   
   framepoint=(unsigned char *)&framehead;
   frametail=get_CRC16_check_sum(framepoint,sizeof(framehead),frametail);
   framepoint=(unsigned char *)&datahead;
   frametail=get_CRC16_check_sum(framepoint,sizeof(datahead),frametail);
   framepoint=(unsigned char *)&imageData;
   frametail=get_CRC16_check_sum(framepoint,sizeof(imageData),frametail);             //CRC16校验   //CRC16校验值计算（部分）
   
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
   }                                                   //发送操作数据  
   framepoint=(unsigned char *)&imageData;
   for(i=0;i<sizeof(imageData);i++)
   {
      UI_SendByte(framepoint);
      framepoint++;             
   }                                               //发送图片帧
   
   
   
   framepoint=(unsigned char *)&frametail;
   for(i=0;i<sizeof(frametail);i++)
   {
      UI_SendByte(framepoint);
      framepoint++;                                                  //发送CRC16校验值
   }
   
   
   UI_Seq++;                                                         //包序号+1
   return 0;
}

/**
 *  @brief          使用自定义UI绘制字符串
 *  @author         LH.Mxy
 *  @param[in]      字符串数据结构体指针
 *  @return         none
 */
void ui_display_string(String_Data* str_data)
{
    frame_header_struct_t framehead;
    UI_Data_Operate operate;

    //1. 帧头内容初始化
    framehead.SOF = 0xA5;
    framehead.Data_Length = sizeof(UI_Data_Operate) + sizeof(String_Data);  //应该为6+45
    framehead.Seq = UI_Seq++;
    framehead.CMD_ID = 0x301;

    //2. 机器人间通讯包头初始化
    operate.Data_ID = 0x110;                //自定义ID,绘制字符串
    operate.Sender_ID = get_robot_id();     //发送端ID
    operate.Receiver_ID = get_client_id();  //接收端ID

    //3. 数据转存
    memcpy(referee_uart_tx_buf[referee_tx_fifo], &framehead, sizeof(frame_header_struct_t));
    memcpy(referee_uart_tx_buf[referee_tx_fifo] + sizeof(frame_header_struct_t), &operate, sizeof(UI_Data_Operate));
    memcpy(referee_uart_tx_buf[referee_tx_fifo] + sizeof(frame_header_struct_t) + sizeof(UI_Data_Operate), str_data, sizeof(String_Data));

    //4. CRC校验
    append_CRC8_check_sum(referee_uart_tx_buf[referee_tx_fifo], 5);
    append_CRC16_check_sum(referee_uart_tx_buf[referee_tx_fifo], sizeof(frame_header_struct_t) + sizeof(UI_Data_Operate) + sizeof(String_Data) + 2);

    //5. DMA发送
  
    usart6_tx_dma_enable(referee_uart_tx_buf[referee_tx_fifo], sizeof(frame_header_struct_t) + sizeof(UI_Data_Operate) + sizeof(String_Data) + 2);
    referee_tx_fifo = referee_tx_fifo == 0 ? 1 : 0;
}
