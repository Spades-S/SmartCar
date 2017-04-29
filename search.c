#include "search.h"
#define ABS(x) ((x>0)?(x):(-x))
#define BYTE0(Temp)  (*(char *)(&Temp))
#define BYTE1(Temp)  (*((char *)(&Temp)+1))
#define BYTE2(Temp)  (*((char *)(&Temp)+2))
#define BYTE3(Temp)  (*((char *)(&Temp)+3))
#define LEFT 0
#define RIGHT 1
#define RANGE_NEAR 3 //寻找最靠近上次边线时的判定范围
int Max_Peak = 0;
float Threshold=35;//判定阈值
int Reference_Width=45; 

int RisingEdgeIndex,FallingEdgeIndex;//寻找到的跳变沿index,FindBoundary 专属
uint8 RisingEdgeCnt=0,FallingEdgeCnt=0,RisingEdge[5]={0},FallingEdge[5]={0};
uint8 Leftlastfind=0,Rightlastfind=0;//上一次是否找到了边线
uint8 Leftlast,Rightlast;
uint8 RoadType=0;
int leftfind,rightfind;//flag表示当前找到的可能是边线
int Right=127,Left=0;//搜线算法找到的左右边线的值
int CCD_Diff[128];//差分值数组
float Middle_Err=0,Middle_Last=0;//中线偏移量
float Correct_Err=0,K_Barraicade=0.5;;
//区分赛道类型进行针对性控制
void RoadType_Distinguish(void){
  EdgeFind();
  Normal();
  if(RoadType==1){
  
    LPLD_GPIO_Output_b(PTE, 10, 0);
  
  }
  
  if(Right-Left <15){ //此时左右边线虽然找到，却不可用
    Right = Rightlast;
    Left = Leftlast;
  
  }
  
    //将赛道信息写入发送数组
  CCD_Draw_Buff[128]=Left-1;
  CCD_Draw_Buff[129]=Right; 
  CCD_Draw_Buff[132]=BYTE0(Max_Peak);
  CCD_Draw_Buff[133]=BYTE1(Max_Peak);
  CCD_Draw_Buff[134]=Leftlastfind;
  CCD_Draw_Buff[135]=Rightlastfind;
  CCD_Draw_Buff[136]=RoadType; 
  Leftlast=Left;              // 更新边界线
  Rightlast=Right;
  if(Rightlastfind||Leftlastfind)//当左线右线能找到其中一个
  {
    Middle_Err=64-(Leftlast+Rightlast)/2.0+1;//计算得到中线偏移量
    if(ABS(Middle_Err-Middle_Last)>2) 
    {
      if(ABS(Middle_Err-Middle_Last)>3)  //可能出现错误采集，对中心偏移量进行微调
      {
        if(Middle_Err>Middle_Last) Middle_Err=Middle_Last+1;
        else Middle_Err=Middle_Last-1;
      }
      else  //在合理采集范围内，对中心偏移量进行更新
      {
        Middle_Err=(Middle_Last+Middle_Err)/2;
        
      }
    
    }
    Middle_Last=Middle_Err;
  }



}

//对常规赛道的处理
void Normal(void){
  uint8 tempwidth=0;
  if(Leftlastfind==0&&Rightlastfind==0){  //如果上一次两条边线均未找到
    Find_BothLine();
    if(Leftlastfind==0&&Rightlastfind==0){
      if(Find_Boundary(LEFT))
      {
        Left=RisingEdge[RisingEdgeIndex];
        Leftlastfind=1; 
      }
      else  Leftlastfind=0;    
      if(Find_Boundary(RIGHT))
       {
        Right=FallingEdge[FallingEdgeIndex];
        Rightlastfind=1;
       }
       else  Rightlastfind=0; 
       if(Leftlastfind&&Rightlastfind)
       {
        if(Right<Left)
        {
         Leftlastfind=0;
         Rightlastfind=0;
        }
       }    
    }
  
  }else{
    if(Leftlastfind){//如果上次找到了左边线 
      if(FindNearest(LEFT,Leftlast)){
        Left=RisingEdge[RisingEdgeIndex];
        Leftlastfind=1;	
        if(Rightlastfind==0){ //上次这次均找到了边线，上次找不到右边线
          if(FallingEdgeCnt){
            for(FallingEdgeIndex=0;FallingEdgeIndex<FallingEdgeCnt;FallingEdgeIndex++){
              rightfind =0;
              if(FallingEdge[FallingEdgeIndex]>Left) {
                tempwidth = FallingEdge[FallingEdgeIndex]-Left;
                if(ABS(tempwidth-Reference_Width)>=2){
                  if(tempwidth>Reference_Width){
                    Reference_Width=Reference_Width+1;
                  }   
                  else{
                    Reference_Width=Reference_Width-1;
                  }
                 Right=Left+Reference_Width;
                 Rightlastfind = 0;                
                }else{
                  Right=FallingEdge[FallingEdgeIndex];
                  Rightlastfind=1;                  
                
                }
                rightfind = 1;
                break;
                
                
              
              }
              
              
              
            }
            
            if(Rightlastfind==0&&rightfind==0){  //此时丢线，利用算法补线
              //Right=Left+Reference_Width;
              Right = 88;

            }
          
          }
          else{  //直接没有下降沿，丢线，进行补线
          
            Right = 88;
          }
          
          
        
        }
      
      }else{
        Leftlastfind=0;
        if(Find_Boundary(LEFT)){
          if(RisingEdge[RisingEdgeIndex]<Leftlast){
            if(Rightlastfind==0){
              RoadType=1; //进入十字后弯道，斜入
            }
          }
        
        
        }else {  //这次连边界都没找到
          if(Rightlastfind==0){  //肯定是进入十字了
            RoadType=1;
          }
        
        
        }
      
      
      
      }
      
    
    
    
    }
    
    if(Rightlastfind){
      if(FindNearest(RIGHT,Rightlast)){
         Right=FallingEdge[FallingEdgeIndex];
         Rightlastfind=1;
         if(Leftlastfind==0){
           if(RisingEdgeCnt>0){
             for(RisingEdgeIndex=RisingEdgeCnt-1;RisingEdgeIndex>=0;RisingEdgeIndex--){
                leftfind=0;
                if(RisingEdge[RisingEdgeIndex]<Right){
                  tempwidth = Right-RisingEdge[RisingEdgeIndex];
                  if (ABS(tempwidth-Reference_Width)>=2){
                    if(tempwidth>Reference_Width){
                      Reference_Width=Reference_Width+1;
                    }else{
                      Reference_Width=Reference_Width-1;
                    }
                     Left=Right-Reference_Width;
                     Leftlastfind = 0;
                  
                  }else{
                     Left=RisingEdge[RisingEdgeIndex];
                     Leftlastfind=1;                  
                  }
                  leftfind=1;
                  break;
                  
                  
                
                }
             
             }
             if(Leftlastfind==0&&leftfind==0){
             
                //Left=Right-Reference_Width;
               Left = 38;

             }

           }
           else{
              //Left=Right-Reference_Width;
              Left = 38;
           }
         
         }
      
      
      }else{
        
        Rightlastfind=0;
        if(Find_Boundary(RIGHT)){
          if(FallingEdge[FallingEdgeIndex]>Rightlast){
            if(Leftlastfind==0){
                RoadType=1; 
            }
          }
        
        }else{
          if(Leftlastfind==0){
          
            RoadType=1; 
          
          }
        
        }
        
        
        
      }
      
      
    
    }
  
  
   if(Rightlastfind&&Leftlastfind)
   {
     
     Reference_Width=Right-Left; 
     if(Reference_Width<35)Reference_Width=35;
   }
  
  
  }
  


}


//寻找跳变沿
void EdgeFind(void){
  int i = 0;//for 的下标
  //对各值进行初始化
  Max_Peak=0;
  RisingEdgeCnt=0;
  FallingEdgeCnt=0;
  for(i=0;i<5;i++)
  {
    RisingEdge[i]=0;
    FallingEdge[i]=0;
  }
  for(i=3;i<128;i++)
  {
    CCD_Diff[i]=CCD_Buff[i]-CCD_Buff[i-3];

    if(ABS(CCD_Diff[i])>Max_Peak) Max_Peak=ABS(CCD_Diff[i]);//寻找差分值的峰值 
  }
  for(i=38;i<88;i++)
  {
    if((CCD_Diff[i]>=CCD_Diff[i-1])&&(CCD_Diff[i]>CCD_Diff[i+1])&&(CCD_Diff[i]>Threshold)) //寻找正的峰值，左边线
    {
      if(RisingEdgeCnt<5)    
      { 
      
         RisingEdge[RisingEdgeCnt]=i;
         RisingEdgeCnt++;
     
        
      }
    } 
    if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold))  //寻找负的峰值，右边线
    {
      if(FallingEdgeCnt<5)    
      {
        
            FallingEdge[FallingEdgeCnt]=i;
            FallingEdgeCnt++;
     

      }
    }
  }
  if(RisingEdgeCnt==0){
     RisingEdge[0]=38;
  }
  
  if(FallingEdgeCnt==0){
    FallingEdge[0]=88;
  }

}


//同时寻找两条边线
void Find_BothLine(void){       
  uint8 i=0,j=0,find=0;//find：是否同时找到了左右边线
  for(j=60;j>20;j--)
  {
    if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) //寻找正的峰值
    {
      break;
    }
  }
  for(i=70;i<110;i++)
  {
    if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold))  //寻找负的峰值
    {
      break;
    }
  }
  if(j>25&&i<105)
  {
    find=1;
    Left=j;
    Right=i;
    Leftlastfind=1;
    Rightlastfind=1;  
  }
  else
  {    
    for(j=10;j<80;j++)         
    {
       if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) 
      {
        for(i=j+1;i<=120;i++) 
        {
          if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold)) 
          {
            if(i-j>25)
            {
               find=1;
               Left=j;
               Right=i;
               Leftlastfind=1;
               Rightlastfind=1; 
            }
            break;
          }
        }
      }
      if(find) break;
    } 
  }
  if(find==0){
    Leftlastfind=0;
    Rightlastfind=0;
  }

}


uint8 Find_Boundary(uint8 mode){
  uint8 find=0;
  if(mode == LEFT){
    if(RisingEdgeCnt!=0)  
    { 
      for(RisingEdgeIndex=RisingEdgeCnt-1;RisingEdgeIndex>=0;RisingEdgeIndex--)
      {
         if(RisingEdge[RisingEdgeIndex]<75)
         {
           find=1;
           break;
         }
      }
    }
  }else{
    if(FallingEdgeCnt!=0)  
    {
     for(FallingEdgeIndex=0;FallingEdgeIndex<FallingEdgeCnt;FallingEdgeIndex++)
     {
      if(FallingEdge[FallingEdgeIndex]>53)
      {
       find=1;
       break;
      }
     }
    }  
  }
  
  return find;
  


}


uint8 FindNearest(uint8 mode, uint8 lastedge){
  uint8 find = 0;
  if(mode == LEFT){
     if(RisingEdgeCnt!=0)  
     {
      for(RisingEdgeIndex=0;RisingEdgeIndex<RisingEdgeCnt;RisingEdgeIndex++)
      {
       if(ABS(lastedge-RisingEdge[RisingEdgeIndex])<=RANGE_NEAR)
       {
         find=1;
         break;
       }
      }
     }  
  
  }else{
    if(FallingEdgeCnt!=0)  
    {
     for(FallingEdgeIndex=0;FallingEdgeIndex<FallingEdgeCnt;FallingEdgeIndex++)
     {
       if(ABS(lastedge-FallingEdge[FallingEdgeIndex])<=RANGE_NEAR)
       {
         find=1;
         break;
       }
     }
    }  
  
  }

  return find;
}


