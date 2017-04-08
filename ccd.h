#ifndef _CCD_H
#define _CCD_H

#include "common.h"
#include "debug.h"

extern GPIO_InitTypeDef gpio_init_struct;
extern ADC_InitTypeDef  adc_init_struct;

/*
//宏定义数据线和时钟线
#define SI_SetVal()  LPLD_GPIO_Output_b(PTB, 8, 1)
#define SI_ClrVal()  LPLD_GPIO_Output_b(PTB, 8, 0)
#define CLK_SetVal() LPLD_GPIO_Output_b(PTB, 7, 1)
#define CLK_ClrVal() LPLD_GPIO_Output_b(PTB, 7, 0)*/

#define SI_SetVal()  LPLD_GPIO_Output_b(PTB, 17, 1)
#define SI_ClrVal()  LPLD_GPIO_Output_b(PTB, 17, 0)
#define CLK_SetVal() LPLD_GPIO_Output_b(PTB, 16, 1)
#define CLK_ClrVal() LPLD_GPIO_Output_b(PTB, 16, 0)
void ccd_GPIO_init(void);
void ccd_ADC_init(void);
void ccd_init(void);
void delay200ns(void);
void CCD_Capture(void);
void CCD_Normalization(void);
#endif