#ifndef REPORT_H_INCLUDED
#define REPORT_H_INCLUDED

#include "struct_typedef.h"

void Report_Data( short *data, u8 len);

void Report_Add(short data);		//添加新数据
void Report(void);					//发送数据
//void usb_report(int cnt,...);
void usb_report(int cnt,float data1,float data2,float data3,float data4);



#endif

