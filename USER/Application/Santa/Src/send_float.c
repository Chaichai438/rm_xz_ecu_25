#include "stdio.h"
#include "stdint.h"
#include "send_float.h"

// 定义一个发送函数(含2个数据)


//int main()
//{
//    float data1 = 100.0f;
//    float data2 = -200.0f;

//	// 发送10次数据
//    for (float i = 0; i < 10; i++)
//    {
//        SendCurve(data1, data2);
//        data1++;
//        data2++;
//    }

//    return 0;
//}

void SendCurve(float data1, float data2,float data3,float data4)
{
    uint8_t dataBuff[(4 + 1) * sizeof(float)];  //缓存大小：(数据数量 + 1) * sizeof(float)
    int cnt = 0;
    
  dataBuff[cnt++]=BYTE0(data1); //第一个数据
  dataBuff[cnt++]=BYTE1(data1);
	dataBuff[cnt++]=BYTE2(data1);
	dataBuff[cnt++]=BYTE3(data1);

  dataBuff[cnt++]=BYTE0(data2); //第二个数据
  dataBuff[cnt++]=BYTE1(data2);
	dataBuff[cnt++]=BYTE2(data2);
	dataBuff[cnt++]=BYTE3(data2);

	dataBuff[cnt++]=BYTE0(data3); //第三个数据
  dataBuff[cnt++]=BYTE1(data3);
	dataBuff[cnt++]=BYTE2(data3);
	dataBuff[cnt++]=BYTE3(data3);
	
	dataBuff[cnt++]=BYTE0(data4); //第四个数据
  dataBuff[cnt++]=BYTE1(data4);
	dataBuff[cnt++]=BYTE2(data4);
	dataBuff[cnt++]=BYTE3(data4);
	
  dataBuff[cnt++]=0xFF;         //结束标志
  dataBuff[cnt++]=0xFF;
	dataBuff[cnt++]=0xFF;
	dataBuff[cnt++]=0xFF;

    // 该函数将dataBuff发送出去，可根据实际替换成你的发送函数
    for(int i = 0; i < cnt; i++)
    {
        printf("%c",dataBuff[i]);
    }
}

#define PRINT(title, fmt, args...) printf("{"#title"}"fmt"\n", ##args)
void send_txt(float data1,float data2,float data3,float data4){

  // 绘图
        PRINT(plotter, "%lf, %lf, %lf,%lf", data1, data2, data3,data4);

}