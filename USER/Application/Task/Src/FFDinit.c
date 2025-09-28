#include "FFDinit.h"
#include "Forward_ctrl.h"
#include "main.h"


FORWARD_CTRL_T YawSpdFFD_t={0};
const float YawSpdFFD_Kf[2]={10,0};


void YawSpdFFD_Init(void)
{
	Forward_Ctrl_init(&YawSpdFFD_t,1,YawSpdFFD_Kf, 8500);

}