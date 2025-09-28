#ifndef SEND_FLOAT32_LITTLE_H
#define SEND_FLOAT32_LITTLE_H
#include "main.h"

// 字节求址宏定义
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

void SendCurve(float data1, float data2,float data3,float data4);
void send_txt(float data1,float data2,float data3,float data4);
#endif