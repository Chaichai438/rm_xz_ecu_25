#ifndef  __A101_H
#define  __A101_H
#include "struct_typedef.h"
#include "can.h"

void A101_Cali(void);
void A101_Get_Info(CAN_HandleTypeDef *hcan,CAN_RxHeaderTypeDef rx_message,uint8_t *A101_data);



#endif
