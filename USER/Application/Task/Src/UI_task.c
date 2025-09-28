/**
  ****************************(C) COPYRIGHT SMU2021 AJI****************************
  * @file       UI_task.c/.h
  * @brief      自定义UI任务
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


#include "UI_task.h"
#include "main.h"
#include "string.h"
#include "freertos.h"
#include "task.h"
#include "math.h"
#include "protocol.h"
#include "referee_st.h"
#include "struct_typedef.h"
#include "usart.h"
#include "st_UI.h"
#include "bsp_buzzer.h"
#include "ammunition_feed_ctrl.h"
#include "voltage_task.h"
#include "detect_task.h"
#include "remote_control.h"

#define UI_NOT_EXTERN   (0)
#define UI_IS_EXTERN    (1)
#define mid 960

extern float dianya;

void float_to_str(char* str, float num,int precision);//将浮点型转换为字符型,第三个参数是小数位数
void Draw(void);//推送图像
Graph_Data graph,graph1, graph2, graph3, graph4, graph5, graph6, graph7,Line_0,Line_1,Line_2,Line_3,Line_4;
String_Data ui_str,moca;
extern RC_ctrl_t rc_ctrl;
extern int shoot_spd;


void userUI_draw_constant_power_allowance(uint8_t en, Graph_Data* graph, fp32 cap_volt)
{
    //1. 将 12.5v - 23v 处理成 [0.0, 1.0]
    cap_volt = (cap_volt - 12.5f) / 10.5f;
    if (cap_volt < 0.0f)
        cap_volt = 0.0f;
    else if (cap_volt > 1.0f)
        cap_volt = 1.0f;

//    //2. 绘制图形
//    //当电压在20v以上时，显示绿色
    if (cap_volt > 0.71429f)
        Line_Draw(graph, "CPN", en == 0 ? UI_Graph_ADD : UI_Graph_Change, 3, UI_Color_Green, 40, 550, 110, (uint16_t)(550 + 820 * cap_volt), 110);
    //当电压在16v以上时，显示黄色
    else if (cap_volt > 0.33333f)
        Line_Draw(graph, "CPN", en == 0 ? UI_Graph_ADD : UI_Graph_Change, 3, UI_Color_Yellow, 40, 550, 110, (uint16_t)(550 + 820 * cap_volt), 110);
    //当电压在16v以下时，显示紫红色
    else
        Line_Draw(graph, "CPN", en == 0 ? UI_Graph_ADD : UI_Graph_Change, 3, UI_Color_Purplish_red, 40, 550, 110, (uint16_t)(550 + 820 * cap_volt), 110);
//				UI_ReFresh(1,graph);

}

void userUI_draw_background(void)
{
    //画恒功率电源余量条 外框，警示线
    Rectangle_Draw(&graph1, "CPB", UI_Graph_ADD, 0, UI_Color_Green, 3, 550, 90, 1370, 130);
    Line_Draw(&graph2, "CLH", UI_Graph_ADD, 0, UI_Color_Green, 3, 1136, 91, 1136, 129);
    Line_Draw(&graph3, "CLL", UI_Graph_ADD, 0, UI_Color_Yellow, 3, 823, 91, 823, 129);

    //画行车辅助线
    Line_Draw(&graph6, "RC1", UI_Graph_ADD, 0, UI_Color_Green, 2, 400, 0, 800, 400);
    Line_Draw(&graph7, "RC2", UI_Graph_ADD, 0, UI_Color_Green, 2, 1520, 0, 1120, 400);
	
	UI_ReFresh(5,graph1,graph2,graph5,graph6,graph7);
	UI_ReFresh(1,graph3);

    vTaskDelay(20);
	

    // *** 等待串口空闲，再延迟一段时间
   

    //画“MODE:"
    string_Draw(&ui_str, "MDT", UI_Graph_ADD, 0, UI_Color_Green, 2, 20, 550, 155, "MODE:");
    ui_display_string(&ui_str);
//    // *** 等待串口空闲，再延迟一段时间
//    
    vTaskDelay(20);

    //画“12.5V”
    string_Draw(&ui_str, "CVE", UI_Graph_ADD, 0, UI_Color_Purplish_red, 2, 10, 550, 85, "12.5v");
    ui_display_string(&ui_str);
    // *** 等待串口空闲，再延迟一段时间
    
    vTaskDelay(20);

    //画“16V”
    string_Draw(&ui_str, "CVL", UI_Graph_ADD, 0, UI_Color_Yellow, 2, 10, 810, 85, "16v");
    ui_display_string(&ui_str);
    // *** 等待串口空闲，再延迟一段时间

    vTaskDelay(20);

    //画“20V”
    string_Draw(&ui_str, "CVH", UI_Graph_ADD, 0, UI_Color_Green, 2, 10, 1121, 85, "20v");
    ui_display_string(&ui_str);
    // *** 等待串口空闲，再延迟一段时间
   
    vTaskDelay(20);

    //画“23V”
    string_Draw(&ui_str, "CVF", UI_Graph_ADD, 0, UI_Color_Green, 2, 10, 1340, 85, "23v");
    ui_display_string(&ui_str);
		
		
	 string_Draw(&ui_str, "SPD", UI_Graph_ADD, 0, UI_Color_Green, 3, 10, 640, 485, "24");
    ui_display_string(&ui_str);
	
	vTaskDelay(20);
	
	Line_Draw(&Line_0,"mid",UI_Graph_ADD,9,UI_Color_Yellow,2,mid,500,mid+50,500);//水平线1
	Line_Draw(&Line_2,"002",UI_Graph_ADD,9,UI_Color_Yellow,2,mid,250,mid,500);//竖直线
	Line_Draw(&Line_1,"001",UI_Graph_ADD,9,4,2,mid,450,mid+40,450);//水平线2	
	Line_Draw(&Line_3,"003",UI_Graph_ADD,9,UI_Color_Yellow,2,mid,400,mid+30,400);//水平线3	
	Line_Draw(&Line_4,"004",UI_Graph_ADD,9,UI_Color_Yellow,2,mid,350,mid+20,350);//水平线4
	
	UI_ReFresh(5,Line_0,Line_1,Line_2,Line_3,Line_4);
	
	vTaskDelay(20);
}

/**************************************************************************************************************/
void UI_task(void const *pvParameters)//绘制ui的函数
{
	/*初始化数据*/


	/**********************************************************************************************************/
		//注：字符型图形只有大写
		// 1920* 1080  左下角为(0,0)
	
/*	Float_Draw(&num,"Yao",UI_Graph_ADD,6,UI_Color_Purplish_red,15,3,3,300,875,yaw_angle_in);
	这里弃用绘制浮点数的原因是底层代码似乎有bug，所以改用浮点数转化为字符型的方法来绘制*/
	
	
	
//	for(u8 i=1;i<10;i++)//清空所有图层(其实客户端的清除效果类似，且之前试过一次似乎没有生效？可能是调用方法不当，日后再试)
//	
//	{
//		UI_Delete(UI_Graph_Del,i);
//	}
//	
//	userUI_draw_constant_power_allowance(UI_NOT_EXTERN, &graph,(uint16_t)dianya);

	while(1)  //注意st_UI.c的id与车对应
	{
		if((RC_Key_Read(RC_KEY_Z)==1))
		{
//			userUI_draw_constant_power_allowance(UI_NOT_EXTERN, &graph,(uint16_t)dianya);
		}
		userUI_draw_background();
//		UI_ReFresh(1,graph);
//		userUI_draw_constant_power_allowance(UI_IS_EXTERN, &graph,(uint16_t)dianya);
				
	}
}






//void Draw(void)
//{
//		

//}






//	/*for(u8 i=1;i<10;i++)//清空所有图层(其实客户端的清除效果类似)
//	{
//		UI_Delete(UI_Graph_Del,i);
//	}*/

//	while(1)
//	{	UI_ReFresh(5,Line_0,Line_1,Line_2,Line_3,Line_4);
//		UI_ReFresh(1,Line_5);
//		Char_ReFresh(Line_C1);
//		Char_ReFresh(Line_C2);
//		Char_ReFresh(Line_C3);
//		Char_ReFresh(Line_C4);
//		Char_ReFresh(Line_C5);
//		Char_ReFresh(Line_ofL);
//		UI_ReFresh(5,D_C1,D_C2,D_C3,D_C4,FS);
//		Char_ReFresh(pitch);
//		Char_ReFresh(yaw);
//		Char_ReFresh(pitch);
//		Char_ReFresh(dr);
//		vTaskDelay(100);
//	}
//}










/*                   //       1920* 1080
ext_client_custom_character_t Line_2;

void UI_task(void const *pvParameters)
{
	memset(&Line_1, 0, sizeof(Line_1));//中心垂线
	memset(&Line_2, 0, sizeof(Line_2));

	float width = 240, y = 610;                              //单位像素
	Line_Draw(&Line_1, "091", UI_Graph_ADD, 9, UI_Color_White, 1, (960 - width * 0.5), y, (960 + width * 0.5), y);               
//	Char_Draw(&Line_2, "092", UI_Graph_Del, 8, UI_Color_White, 14, 2, 1, (960 + width * 0.5) + 5, y, "2m");
//	Line_Draw(&Line_1, "091", UI_Graph_ADD, 9, UI_Color_White, 1, (960 - width * 0.5), y, (960 +width * 0.5), y);               //2m
//	Char_Draw(&Line_2, "092", UI_Graph_ADD, 8, UI_Color_White, 14, 2, 1, (960 + width * 0.5)+5, y, "2m");

	width = 180; y = 680;
//	Line_Draw(&Line_1, "091", UI_Graph_Del, 9, UI_Color_White, 1, (960 - width * 0.5), y, (960 + width * 0.5), y);
//	Char_Draw(&Line_2, "092", UI_Graph_Del, 8, UI_Color_White, 14, 2, 1, (960 + width * 0.5) + 5, y, "4m");
//	Line_Draw(&Line_1, "091", UI_Graph_ADD, 9, UI_Color_White, 1, (960 - width * 0.5), y, (960 + width * 0.5), y);               //4m
//	Char_Draw(&Line_2, "092", UI_Graph_ADD, 8, UI_Color_White, 14, 2, 1, (960 + width * 0.5)+5, y, "4m");

//	width = 120; y = 750;
//	Line_Draw(&Line_1, "091", UI_Graph_Del, 9, UI_Color_White, 1, (960 - width * 0.5), y, (960 + width * 0.5), y);
//	Char_Draw(&Line_2, "092", UI_Graph_Del, 8, UI_Color_White, 14, 2, 1, (960 + width * 0.5) + 5, y, "6m");
//    Line_Draw(&Line_1, "091", UI_Graph_ADD, 9, UI_Color_White, 1, (960 - width * 0.5), y, (960 + width * 0.5), y);               //6m
//	Char_Draw(&Line_2, "092", UI_Graph_ADD, 8, UI_Color_White, 14, 2, 1, (960 + width * 0.5)+5, y, "6m");

//	width = 60; y = 820;
//	Line_Draw(&Line_1, "091", UI_Graph_Del, 9, UI_Color_White, 1, (960 - width * 0.5), y, (960 + width * 0.5), y);
//	Char_Draw(&Line_2, "092", UI_Graph_Del, 8, UI_Color_White, 14, 2, 1, (960 + width * 0.5) + 5, y, "8m");
//	Line_Draw(&Line_1, "091", UI_Graph_ADD, 9, UI_Color_White, 1, (960 - width * 0.5), y, (960 + width * 0.5), y);                //8m
//	Char_Draw(&Line_2, "092", UI_Graph_ADD, 8, UI_Color_White, 14, 2, 1, (960 + width * 0.5)+5, y, "8m");

//	Line_Draw(&Line_1, "091", UI_Graph_Del, 9, UI_Color_White, 1, 960, 580, 960, 580);
//	Line_Draw(&Line_1, "091", UI_Graph_ADD, 9, UI_Color_White, 1, 960, 580, 960, 580);                //竖线

	while(1)
	{
                             //单位像素
	Line_Draw(&Line_1, "091", UI_Graph_ADD, 9, UI_Color_Purplish_red, 1, 960, 330, 960, 620);     
		Char_Draw(&Line_2, "092", UI_Graph_ADD, 8, UI_Color_Purplish_red,  1, 5,2, 900, 300,"3m");

//			Char_Draw(&Line_2, "092", UI_Graph_Del, 8, UI_Color_White, 14, 2, 1, (960 + width * 0.5) + 5, y, "4m");
//		UI_ReFresh(1, Line_1);
//		UI_ReFresh(1, Line_2);
		vTaskDelay(100);		
	}
}
*/






