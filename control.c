#include "control.h"

PID PID_Speed,PID_Turn;
float K_CrossRoad=0,P_CrossRoad=0;
float SetSpeed=0,Barraicade_Num=1;
/****
* µç»úPWMÊä³ö
****/
void Motor_Output(void)
{
  LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5,2500);
    
  LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7,2500);    //1900 2500 2800
}



