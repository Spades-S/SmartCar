#ifndef _INIT_H
#define _INIT_H

#include "common.h"

void Right_Encoder_Init(void);
void Pit_Init(void);
extern void pit_isr(void);
extern void gpio_isr(void);
void Motor_Init(void);
void delay(uint16);
void Servo_Init(void);
void Left_Encoder_Init(void);
void Para_Init(void);
void GPIO_Init(void);
void Voltage_Adc_init(void);


#endif
