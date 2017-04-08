#include "init.h"

//��������
FTM_InitTypeDef ftm_init_struct;
PIT_InitTypeDef pit_init_struct;
GPIO_InitTypeDef gpio_init_struct;
ADC_InitTypeDef  adc_init_struct;
LPTMR_InitTypeDef lptmr_init_struct;


void Motor_Init(void)
{
        //���ģ��ĳ�ʼ��
    ftm_init_struct.FTM_Ftmx = FTM0;
    ftm_init_struct.FTM_Mode = FTM_MODE_PWM;
    ftm_init_struct.FTM_PwmFreq = 2500;
    LPLD_FTM_Init(ftm_init_struct);
    

    LPLD_FTM_PWM_Enable(FTM0, FTM_Ch4, 50, PTD4, ALIGN_LEFT); //pwm3
    LPLD_FTM_PWM_Enable(FTM0, FTM_Ch5, 50, PTD5, ALIGN_LEFT); //pwm2
    LPLD_FTM_PWM_Enable(FTM0, FTM_Ch6, 50, PTD6, ALIGN_LEFT); //pwm1
    LPLD_FTM_PWM_Enable(FTM0, FTM_Ch7, 50, PTD7, ALIGN_LEFT); //pwm0

}


void Servo_Init(void)
{
    ftm_init_struct.FTM_Ftmx = FTM2;
    ftm_init_struct.FTM_Mode = FTM_MODE_PWM;
    ftm_init_struct.FTM_PwmFreq = 50;   //����Ƶ��Ϊ50Hz
    LPLD_FTM_Init(ftm_init_struct);
     
    LPLD_FTM_PWM_Enable(FTM2, FTM_Ch1, 660,PTB19, ALIGN_LEFT);   
}

void Pit_Init(void)
{
  //���ö�ʱ�����жϲ���
  pit_init_struct.PIT_Pitx = PIT0;
  pit_init_struct.PIT_PeriodMs = 1;    //1�������һ���ж�
  pit_init_struct.PIT_Isr = pit_isr;
  //��ʼ��PIT
  LPLD_PIT_Init(pit_init_struct);
  //ʹ��PIT��ʱ�ж�
  LPLD_PIT_EnableIrq(pit_init_struct);
}

void Voltage_Adc_init(void)
{
  adc_init_struct.ADC_Adcx = ADC1;
  adc_init_struct.ADC_BitMode = SE_10BIT;
  adc_init_struct.ADC_CalEnable = TRUE; //ʹ���Զ�У׼
  LPLD_ADC_Init(adc_init_struct); 
  
  LPLD_ADC_Chn_Enable(ADC1, AD14);

}

void GPIO_Init(void)
{
    //��ʼ������B1-B6
    gpio_init_struct.GPIO_PTx = PTA;
    gpio_init_struct.GPIO_Dir = DIR_INPUT;
    gpio_init_struct.GPIO_Pins = GPIO_Pin9|GPIO_Pin10|GPIO_Pin11|GPIO_Pin12|GPIO_Pin13|GPIO_Pin14;
    gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP;
    LPLD_GPIO_Init(gpio_init_struct); 

    //��ʼ�����뿪��
    gpio_init_struct.GPIO_PTx = PTB;
    gpio_init_struct.GPIO_Dir = DIR_INPUT;
    gpio_init_struct.GPIO_Pins = GPIO_Pin2|GPIO_Pin3|GPIO_Pin4|GPIO_Pin5;
    gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP;
    LPLD_GPIO_Init(gpio_init_struct); 

    
    //��ʼ��LED
    gpio_init_struct.GPIO_PTx = PTE;
    gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
    gpio_init_struct.GPIO_Output = OUTPUT_H;
    gpio_init_struct.GPIO_Pins = GPIO_Pin10|GPIO_Pin11|GPIO_Pin12;
    gpio_init_struct.GPIO_PinControl =  OUTPUT_DSH;
    LPLD_GPIO_Init(gpio_init_struct);

    
    
    
    //��ʼ��OLED��Ļ
    gpio_init_struct.GPIO_PTx = PTA;
    gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
    gpio_init_struct.GPIO_Output = OUTPUT_H;
    gpio_init_struct.GPIO_Pins = GPIO_Pin16 | GPIO_Pin17 | GPIO_Pin24 | GPIO_Pin25;

    LPLD_GPIO_Init(gpio_init_struct); 




}

