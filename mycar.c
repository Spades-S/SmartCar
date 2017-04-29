#include "mycar.h"
void pit_isr(void);
void servo_angle_control( int angle);
void main (void)
{
  GPIO_Init();
  Motor_Init();
  Servo_Init();
  Voltage_Adc_init();
  OLED_Init();
  ccd_init();
  Pit_Init();
  while(1)
  {
     If_Button_Press();
     OLED_Draw_UI();
     Send_Begin();
     Send_CCD();
  } 
}


void servo_angle_control( int angle) 
{
  uint16 DutyRation = 0;
  DutyRation = (uint16)(420 - angle*6.4);//5.7   6.4  6.6
  if(DutyRation>=516) 
  {DutyRation=516;}
  if(DutyRation<=324)
  { DutyRation=324;}
    
  LPLD_FTM_PWM_ChangeDuty(FTM2, FTM_Ch1, DutyRation);  //舵机角度控制//660
}


void pit_isr(void)
{
  static uint8 part=0;

  
  part+=1;

  
  switch(part)
  {
    case 1:

      break;
      
    case 2: 
      //CCD采集
      CCD_Capture(); //ccd采集
      
      

      break;
    case 3:
      CCD_Normalization();//归一化程序
      
      break;
    case 4://道路识别
      RoadType_Distinguish();

      break;
    case 5:

      part=0;
      break;
      
    default:
      break;
    
  
  
  }
  
  Motor_Output();
  servo_angle_control(0-Middle_Err);
}



void Send_CCD(void) 
{
  uint8 i,num=136;
  
   //通过串口发送一个字节的数据，下同
  LPLD_UART_PutChar(UART4,0x55);

  LPLD_UART_PutChar(UART4,0xaa);

  LPLD_UART_PutChar(UART4,0xaf);

  LPLD_UART_PutChar(UART4,num);
  for(i=0;i<num;i++)              //CCD
  {
   
   LPLD_UART_PutChar(UART4,CCD_Draw_Buff[i]);
  }
  //帧尾，必须发送
  LPLD_UART_PutChar(UART4,0x0f);
}

void Send_Begin()
{
  LPLD_UART_PutChar(UART4,0x55);
  LPLD_UART_PutChar(UART4,0xaa);
  LPLD_UART_PutChar(UART4,0xae);


}



