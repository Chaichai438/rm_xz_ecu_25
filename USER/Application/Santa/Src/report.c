#include "report.h"
#include "usart.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <stdarg.h>
#include "string.h"


void usart3_send_char(u8 c);
void usart3_niming_report( u8 fun, u8 *data, u8 len);

unsigned char DATA_buf[20];
static short datas[20];
static short data_index = 0;


//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart3_niming_report( u8 fun, u8 *data, u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)
		return;	//最多28字节数据 
	
	send_buf[len+3] = 0;	//校验数置零
	send_buf[0]		= 0X88;	//帧头
	send_buf[1]		= fun;	//功能字
	send_buf[2]		= len;	//数据长度
	
	for(i=0;i<len;i++)
		send_buf[3+i]=data[i];			//复制数据
	
	for(i=0;i<len+3;i++)
		send_buf[len+3]+=send_buf[i];	//计算校验和	
	//for(i=0;i<len+4;i++)
		//usart3_send_char(send_buf[i]);	//发送数据到串口1 
	CDC_Transmit_FS(send_buf,len+4);
}


//发送要显示波形的数据
void Report_Data( short *data, u8 len)
{
	u8 tbuf[20];
	u8 t;
	
	if( len > 20)		//长度限制
		return ;
	
	for( t = 0; t < len * 2; ++t)
	{
		if( t % 2 == 0)
			tbuf[t] = ( data[t / 2] >> 8) & 0xff; 
		else tbuf[t] = data[ t / 2] & 0xff;
	}
	//printf("%d,data=%d\r\n",tbuf[2] << 8 | tbuf[3],data[1]);
	usart3_niming_report( 0xa1, tbuf, len * 2);//自定义帧,0XA1
}	







//数据转换,通过usb打印
/**
  * @brief          
  * @param[in]      cnt：传入参数的个数
  * @param[in]		要显示的数据（当前为float型）
  * @retval         
  */

/*
void usb_report(int cnt,...)
{
    static va_list ap;
    uint16_t i,n;
	//len = 0;
	uint8_t *pTemp;
	float pid_Data;
	
	va_start(ap, cnt);
//	len = vsprintf((char *)DATA_buf, cnt, ap);
	for(i=0;i<cnt;i++)
   {
      pid_Data=(float)va_arg(ap,double);
      
      pTemp=(unsigned char *)&pid_Data;
      
      for(n=0;n<4;n++)
      {
			DATA_buf[4*cnt+n]= pTemp[3-n];
      }                                               //发送图片帧
   }
	
    va_end(ap);
   
	usart3_niming_report( 0xa1, DATA_buf, 4 * cnt);//自定义帧,0XA1
}
*/


void usb_report(int cnt,float data1,float data2,float data3,float data4)
{
    static va_list ap;
    uint16_t n;
	uint8_t *pTemp;      
	pTemp=(unsigned char *)&data1;
	for(n=0;n<4;n++)
	{
		DATA_buf[n]= pTemp[3-n];
	}                                               //发送图片帧
	
	pTemp=(unsigned char *)&data2;
	for(n=0;n<4;n++)
	{
		DATA_buf[4+n]= pTemp[3-n];
	}   
	
	pTemp=(unsigned char *)&data3;
	for(n=0;n<4;n++)
	{
		DATA_buf[4*2+n]= pTemp[3-n];
	}
	
	pTemp=(unsigned char *)&data4;
	for(n=0;n<4;n++)
	{
		DATA_buf[4*3+n]= pTemp[3-n];
	}   
		
	usart3_niming_report( 0xa1, DATA_buf, 4*cnt);//自定义帧,0XA1
}

