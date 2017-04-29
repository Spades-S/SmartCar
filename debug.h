#ifndef _DEBUG_H_

#include "common.h"

#include "oled.h"
#include "control.h"

#define _DEBUG_H_
#define BYTE0(Temp)  (*(char *)(&Temp))
#define BYTE1(Temp)  (*((char *)(&Temp)+1))
#define BYTE2(Temp)  (*((char *)(&Temp)+2))
#define BYTE3(Temp)  (*((char *)(&Temp)+3))

#define BT1_In LPLD_GPIO_Input_b(PTA,14)  //right
#define BT2_In LPLD_GPIO_Input_b(PTA,13)  //cancel
#define BT3_In LPLD_GPIO_Input_b(PTA,12)  //down
#define BT4_In LPLD_GPIO_Input_b(PTA,11)  //up
#define BT5_In LPLD_GPIO_Input_b(PTA,10)  //left
#define BT6_In LPLD_GPIO_Input_b(PTA,9)   //yes

extern void delay200ns(void);
extern int Right,Left;

extern float K_CrossRoad,Correct_Err,P_CrossRoad,K_Barraicade;
extern PID PID_Speed,PID_Turn;
extern float Middle_Err;//����ƫ����
extern uint8 CCD_Draw_Buff[150];
extern float SetSpeed,Barraicade_Num;
extern float SetSpeed,Brightness,CCD_Offset;
extern float Threshold,Barraicade_Num;
extern uint8 Leftlastfind,Rightlastfind;
extern int Right,Left,Max_Peak;
extern uint16 Max_Value;
void OLED_Draw_UI();
void Draw_CCD();
void Delay_2ms(void);
void If_Button_Press(void);
#endif

