#ifndef _MYCAR_H_
#define _MYCAR_H_
#include "common.h"
#include "init.h"
#include "ccd.h"
#include "oled.h"
#include "search.h"
#include "control.h"
void Send_Begin();
void Send_CCD(void);
extern uint8 CCD_Draw_Buff[150];
extern float  Middle_Err;

#endif