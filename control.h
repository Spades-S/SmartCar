#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "common.h"

typedef struct PID{float P,pout,I,iout,D,dout,OUT;} PID;
void Motor_Output(void);
#endif