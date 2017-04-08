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

  } 
}


void servo_angle_control( int angle) 
{
  uint16 DutyRation = 0;
  DutyRation = (uint16)(660 - angle*6.0);
  if(DutyRation>=753) 
  {DutyRation=753;}
  if(DutyRation<=580)
  { DutyRation=580;}
    
  LPLD_FTM_PWM_ChangeDuty(FTM2, FTM_Ch1, DutyRation);  //����Ƕȿ���//660
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
      //CCD�ɼ�
      CCD_Capture(); //ccd�ɼ�
      
      

      break;
    case 3:
      CCD_Normalization();//��һ������
      
      break;
    case 4://��·ʶ��

      break;
    case 5:

      part=0;
      break;
      
    default:
      break;
    
  
  
  }

}



void Send_CCD(void) 
{
  uint8 i,num=136;
  
   //ͨ�����ڷ���һ���ֽڵ����ݣ���ͬ
  LPLD_UART_PutChar(UART4,0x55);

  LPLD_UART_PutChar(UART4,0xaa);

  LPLD_UART_PutChar(UART4,0xaf);

  LPLD_UART_PutChar(UART4,num);
  for(i=0;i<num;i++)              //CCD
  {
   
   LPLD_UART_PutChar(UART4,CCD_Draw_Buff[i]);
  }
  //֡β�����뷢��
  LPLD_UART_PutChar(UART4,0x0f);
}

void Send_Begin()
{
  LPLD_UART_PutChar(UART4,0x55);
  LPLD_UART_PutChar(UART4,0xaa);
  LPLD_UART_PutChar(UART4,0xae);


}



