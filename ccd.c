#include "ccd.h"

uint8 CCD_Draw_Buff[150]={0};
uint16 CCD_Buff[128] = { 0 };
float  CCD_Offset=200;           //����ѹ��С    ��Ƭ��12λAD��ɫ��������ֵ��200-1200����
uint16 Max_Value=0;

float Brightness=0;


/****
**��ʼ��CCD����
****/

void ccd_init(void)
{
  ccd_GPIO_init();
  ccd_ADC_init();
}



/***
*��ʼ��CCD�����ߺ�ʱ���ߵ�GPIO 
***/
void ccd_GPIO_init(void)
{
 /* 
  gpio_init_struct.GPIO_PTx = PTB;
  gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
  gpio_init_struct.GPIO_Output = OUTPUT_H;
  gpio_init_struct.GPIO_Pins = GPIO_Pin7 | GPIO_Pin8;
  LPLD_GPIO_Init(gpio_init_struct); //SI��CLK
  */
  
 
  gpio_init_struct.GPIO_PTx = PTB;
  gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
  gpio_init_struct.GPIO_Output = OUTPUT_H;
  gpio_init_struct.GPIO_Pins = GPIO_Pin16 | GPIO_Pin17;
  LPLD_GPIO_Init(gpio_init_struct); //SI��CLK*/
}


/***
*��ʼ��ADC�����ȣ�����12λ
***/
void ccd_ADC_init(void)
{
  adc_init_struct.ADC_Adcx = ADC1;
  adc_init_struct.ADC_BitMode = SE_12BIT;
  adc_init_struct.ADC_CalEnable = TRUE; //ʹ���Զ�У׼
  LPLD_ADC_Init(adc_init_struct); 
  
  LPLD_ADC_Chn_Enable(ADC1, AD15);
}

void delay200ns(void)
{
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");//��10�� һ��asm("nop")��20ns;��Ƶ50MHz
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
}
/***
*CCD�ɼ�����
***/
void CCD_Capture(void)
{
    unsigned char i;
    
    SI_SetVal();
    delay200ns();
    CLK_SetVal();
    delay200ns();
    SI_ClrVal();
    delay200ns();  
    //�ɼ���һ����
    CCD_Buff[0] = LPLD_ADC_Get(ADC1, AD15);
    Max_Value=CCD_Buff[0];
    CLK_ClrVal();
    //�ɼ���2-128
    for (i = 1; i < 128; i++)
    {
        delay200ns();      
        CLK_SetVal();
        delay200ns();
        
    
        CCD_Buff[i] = LPLD_ADC_Get(ADC1, AD15);
        if((Right-Left>20)&&Leftlastfind&&Rightlastfind)             //�ϴ����ұ��߶�������ôӦ��������5��������������ȣ��������������Ƶ�
        {
         if((i>Left)&&(i<Right))      if(Max_Value<CCD_Buff[i]) Max_Value=CCD_Buff[i]; 
        } 
        else
        {
          if(Max_Value<CCD_Buff[i])  Max_Value=CCD_Buff[i];
        }
        CLK_ClrVal();
    }
    delay200ns();
    CLK_SetVal();
    delay200ns();
    CLK_ClrVal();
}

void CCD_Normalization(void)
{
  uint8 i=0;
  float ratio;//���� 
  int temp;
  if(Max_Value>CCD_Offset)
  ratio = 127.0/(Max_Value-CCD_Offset);
  else ratio=0;
  for(i=0;i<128;i++)
  {
    if(CCD_Buff[i]>CCD_Offset)
    temp=CCD_Buff[i]-CCD_Offset;
    else temp=0;
    CCD_Buff[i] = (int)(temp*ratio);
    if(CCD_Buff[i]>126) CCD_Buff[i]=126;
    CCD_Draw_Buff[i]=CCD_Buff[i];
  }
  CCD_Draw_Buff[130] = BYTE0(Max_Value);
  CCD_Draw_Buff[131] = BYTE1(Max_Value);  
  

}

