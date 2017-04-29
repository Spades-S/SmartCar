#include "debug.h"

uint8 Page_Index=0,Para_Index=0,Para_Checked=0,Para_Choice=0;
uint8 Display_Edge=1;
uint8 Step_Index=2;
float Step[6]={0.0001,0.001,0.01,0.1,1.0,10.0}; 
float Voltage=0;
char Para_Name[5][12]={"PID_SPEED.P\0",
"PID_SPEED.I\0","PID_SPEED.D\0","PID_DIREC.P\0","PID_DIREC.I\0"};
char Para_Name1[5][12]={"PID_DIREC.D\0","Brightness\0","K_Barrai\0","Correct_Err\0","K_CrossRoad\0"};
char Para_Name2[3][12]={"SetSpeed\0","P_CrossRoad\0","SAVE\0"};
char Para_Name3[2][12]={"Threshold\0","CCDOffset"};
//将PID变量的地址加到指针数组里
float *Control_Para0[5]={&PID_Speed.P,&PID_Speed.I,&PID_Speed.D,&PID_Turn.P,&PID_Turn.I};
float *Control_Para1[5]={&PID_Turn.D,&Brightness,&K_Barraicade,&Correct_Err,&K_CrossRoad};
float *Control_Para2[2]={&SetSpeed,&P_CrossRoad};
float *Control_Para3[2]={&Threshold,&CCD_Offset};
/********绘制界面********/
void OLED_Draw_UI()
{
  uint8 i;
  
  if(Page_Index!=3)
  {
     OLED_P6x8Str(0,0,"Voltage=");                          //显示电池电压
     OLED_PrintValueF(48, 0,Voltage,3);     
     OLED_P6x8Str(0,1,"Step=");
     OLED_PrintValueF(36, 1,Step[Step_Index],5); 
     OLED_Set_Pos(122,7);
     OLED_P6x8Char(Page_Index+48);                         //写出页面序号
  
  }
  
  
  /*************************************************************/
  if(Page_Index==0) //第0页 显示PID_Speed PID_Turn
  {
    for(i=0;i<5;i++)
    {
      if(i==Para_Index && Para_Choice==1 &&  Para_Checked==0 )
      {
        reverse=1;
        OLED_P6x8Str(0,i+3,Para_Name[i]);   //将参量名反转显示
        reverse=0;
      
      
      }
      else
        OLED_P6x8Str(0,i+3,Para_Name[i]);
      
      
      if(i==(Para_Index) && Para_Checked==1)
      {
        reverse=1;
        OLED_PrintValueF(72, i+3,*Control_Para0[i],5);
        reverse=0;
      }
      else OLED_PrintValueF(72, i+3,*Control_Para0[i],5);

    
    }
  
  }
  else if(Page_Index==1)  //第1页信息  Delta信息， setspeed ，模糊参数
  {
    for(i=0;i<5;i++)
    {
      if(i==Para_Index && Para_Choice==1 &&  Para_Checked==0 )
      {
        reverse=1;
        OLED_P6x8Str(0,i+3,Para_Name1[i]);   //将参量名反转显示
        reverse=0;
      
      
      }
      else
        OLED_P6x8Str(0,i+3,Para_Name1[i]);
      
      
      if(i==(Para_Index) && Para_Checked==1)
      {
        reverse=1;
        OLED_PrintValueF(72, i+3,*Control_Para1[i],5);
        reverse=0;
      }
      else OLED_PrintValueF(72, i+3,*Control_Para1[i],5);

    
    }
    

  }
  else if(Page_Index==2)  //第2页显示整形变量 Threshold
  {
   for(i=0;i<3;i++)
    {
      if(i==Para_Index && Para_Choice==1 &&  Para_Checked==0 )
      {
        reverse=1;
        OLED_P6x8Str(0,i+3,Para_Name2[i]);   //将参量名反转显示
        reverse=0;
      
      
      }
      else
        OLED_P6x8Str(0,i+3,Para_Name2[i]);
      
      if(i<=1)
      {
        if(i==(Para_Index) && Para_Checked==1)
        {
          reverse=1;
          OLED_PrintValueF(72, i+3,*Control_Para2[i],5);

          reverse=0;
        }
        else OLED_PrintValueF(72, i+3,*Control_Para2[i],5);     
      
      
      
      }


    
    }
      
  
  
  
  }
  else if(Page_Index==3)
  {
    Draw_CCD();
  
  }
  else if(Page_Index==4)
  {
    for(i=0;i<2;i++)
    {
      if(i==Para_Index && Para_Choice==1 &&  Para_Checked==0 )
      {
        reverse=1;
        OLED_P6x8Str(0,i+3,Para_Name3[i]);   //将参量名反转显示
        reverse=0;
      
      
      }
      else
        OLED_P6x8Str(0,i+3,Para_Name3[i]);
      
      
      if(i==(Para_Index) && Para_Checked==1)
      {
        reverse=1;
        OLED_PrintValueF(72, i+3,*Control_Para3[i],5);
        reverse=0;
      }
      else OLED_PrintValueF(72, i+3,*Control_Para3[i],5);

    
    }
  
  
  
  }
}
void Draw_CCD()
{
  uint8 i,j,line,dot;
  uint8 temp=0;
  for(j=0;j<128;j++)
  {  

    temp=32-(uint8)(CCD_Draw_Buff[j]>>2);  //除以4
    line=temp/8;
    dot =temp%8; 

    for(i=0;i<4;i++)
    {
        OLED_Set_Pos(j,i);
      if(Display_Edge&&((CCD_Draw_Buff[128]!=0&& CCD_Draw_Buff[128]==j)||( CCD_Draw_Buff[129]!=128&& CCD_Draw_Buff[129]==j)))
      {
       if( CCD_Draw_Buff[128]==j) 
       { 
         if(Leftlastfind)     OLED_WrDat(0xff,0);  
         else                 OLED_WrDat(0xaa,0); 
       }
       else 
       { 
         if(Rightlastfind)  OLED_WrDat(0xff,0);  
         else               OLED_WrDat(0xaa,0);  
       }
             
      }
      else
      { 
       if(i==line)
       OLED_WrDat(1<<dot,0);
       else 
       OLED_WrDat(0,0);
      }
    }
  }
   OLED_P6x8Str(1,4,"Left=");
   OLED_PrintValueF(31, 4,Left,3);
   OLED_P6x8Str(64,4,"Right="); 
   OLED_PrintValueF(100, 4, Right,3);
   OLED_P6x8Str(0,5,"Middle_Err=");
   OLED_PrintValueF(72, 5,Middle_Err,3);
   OLED_P6x8Str(0,6,     "Max_Peak  =");
   OLED_PrintValueF(72, 6,Max_Peak,3);     
   OLED_P6x8Str(0,7,  "Max_Value =");
   OLED_PrintValueF(72, 7,Max_Value,4);
   OLED_Set_Pos(122,7);
   OLED_P6x8Char('L');      
                      
}


void If_Button_Press(void)
{

  if(BT6_In==0)  //按键yes
  {
    Delay_2ms();//消除抖动
    if(BT6_In==0)
    {   if(Page_Index==4)
        {
          if(Para_Index<2)
          {
            if(Para_Choice==0)  Para_Choice=1;  //  选择参量   
            else
            {
              if(Para_Checked==0) Para_Checked=1;
              else Para_Checked=0;       
            }
          
          }
        
        
        }
        else if(Page_Index==2)
        {
          if(Para_Index<=1)
          {
            if(Para_Choice==0)  Para_Choice=1;  //  选择参量   
            else
            {
              if(Para_Checked==0) Para_Checked=1;
              else Para_Checked=0;       
            }
          
          }
          else if(Para_Index==2)
          {
              
              
              ////////////////////save e2prom
              //DebugData_Save();
              OLED_PrintValueI(72, 5,1);
          
          }
        
        }
        else if(Page_Index<=1)
        {
          if(Para_Index<5)
          {
            if(Para_Choice==0)  Para_Choice=1;  //  选择参量   
            else
            {
              if(Para_Checked==0) Para_Checked=1;
              else Para_Checked=0;       
            }
          
          }        
        
        
        }
    
    }
    while(BT6_In==0);
  
  }
  //btn_cancel 
  if(BT2_In==0)
  {
     Delay_2ms();
     if(BT2_In==0)
     {
        if(Para_Choice==1) Para_Choice=0;
        if(Para_Checked==1) Para_Checked=0;
        if(Page_Index==3)
        {
          if(Display_Edge) Display_Edge=0;
          else Display_Edge=1;
        
        }
     
     }
     while(BT2_In==0);
  
  }
  
  //btn_left
  if(BT5_In==0)
  {
    Delay_2ms();
    if(BT5_In==0)
    {
      if(Para_Checked)//有参数被选中时，左右键改变步长
      {
        if(Step_Index==0)
        {
          Step_Index=0;
         
        }
        else Step_Index--;
      
      }
      else   //没有参数被选中时翻页
      {
        Para_Index=0;
        if(Page_Index==0) Page_Index=4;
        else Page_Index--;
        OLED_Fill(0);  //翻页后首先清空界面
      
      }
    
    }
    while(BT5_In==0);
  
  
  
  
  }
  
  
  //btn_right
  if(BT1_In==0)
  {
    Delay_2ms();
    if(BT1_In==0)
    {
      if(Para_Checked)
      {
        if(Step_Index==5)
        {
          Step_Index=5;
        }
        else Step_Index++;
      
      
      }
      else
      {
        Para_Index=0;
        if(Page_Index==4) 
        {Page_Index=0;}
        else Page_Index++;
        OLED_Fill(0);
      
      }
    
    
    
    }
    while(BT1_In==0);
  
  
  }
  
  //btn_up
  if(BT4_In==0)
  {
    Delay_2ms();
    if(BT4_In==0)
    {
      if(Para_Choice==1)
      {
        if(Page_Index<=1)
        {
          if(Para_Checked==0)
          {
            if(Para_Index==0) Para_Index=4;
            else Para_Index--;
          }
          else 
          {
            if(Page_Index==0 && Para_Index<=4)
            {
              *Control_Para0[Para_Index]+=Step[Step_Index];
            
            }
            else if(Page_Index==1 && Para_Index<=4)
            {
            
              *Control_Para1[Para_Index]+=Step[Step_Index];
            
            }
          
          }
        
        
        
        }
        else if(Page_Index==2)
        {
          if(Para_Checked==0)
          {
              if(Para_Index==0)  Para_Index=2;
              else Para_Index--;

          }
          else
          {
            if(Para_Index<=1)
            *Control_Para2[Para_Index]+=Step[Step_Index];
            
          }

        }
        else if(Page_Index==4)
        {
          if(Para_Checked==0)
          {
              if(Para_Index==0)  Para_Index=1;
              else Para_Index--;
              

          }
          else
          {
            if(Para_Index<2)
            *Control_Para3[Para_Index]+=Step[Step_Index];
            
          }
        
        
        }

      }
      
      
      
      
    }
    while(BT4_In==0);
  
  }
  
  //btn_down
  if(BT3_In==0)
  {
    Delay_2ms();
    if(BT3_In==0)
    {
    if(Para_Choice==1)
      {
        if(Page_Index<=1)
        {
          if(Para_Checked==0)
          {
            if(Para_Index==4) Para_Index=0;
            else Para_Index++;
          }
          else 
          {
            if(Page_Index==0 && Para_Index<=4)
            {
              *Control_Para0[Para_Index]-=Step[Step_Index];
            
            }
            else if(Page_Index==1 && Para_Index<=4)
            {
            
              *Control_Para1[Para_Index]-=Step[Step_Index];
            
            }
          
          }
        
        
        
        }
        else if(Page_Index==2)
        {
          if(Para_Checked==0)
          {
            if(Para_Index==2) Para_Index=0;
            else Para_Index++;
          }
          else
          {
            if(Para_Index<=1)
            *Control_Para2[Para_Index]-=Step[Step_Index];
            
          }

        }
        else if(Page_Index==4)
        {
        
          if(Para_Checked==0)
          {
              if(Para_Index==1) Para_Index=0;
            else Para_Index++;
              

          }
          else
          {
            if(Para_Index<2)
            *Control_Para3[Para_Index]-=Step[Step_Index];

            
          }
      

        }
      
      
      }
      
      
      
      
    }
  
  
  
  
  
  while(BT3_In==0);
  }
  





}


void Delay_2ms(void)
{
    int i;
    for(i=0;i<10;i++)
    {
      delay200ns();
    }

}
