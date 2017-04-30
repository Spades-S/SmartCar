#include "search.h"
#define ABS(x) ((x>0)?(x):(-x))
#define BYTE0(Temp)  (*(char *)(&Temp))
#define BYTE1(Temp)  (*((char *)(&Temp)+1))
#define BYTE2(Temp)  (*((char *)(&Temp)+2))
#define BYTE3(Temp)  (*((char *)(&Temp)+3))
#define LEFT 0
#define RIGHT 1
#define RANGE_NEAR 3 //Ѱ������ϴα���ʱ���ж���Χ
int Max_Peak = 0;
float Threshold=35;//�ж���ֵ
int Reference_Width=40; 

int RisingEdgeIndex,FallingEdgeIndex;//Ѱ�ҵ���������index,FindBoundary ר��
uint8 RisingEdgeCnt=0,FallingEdgeCnt=0,RisingEdge[5]={0},FallingEdge[5]={0};
uint8 Leftlastfind=0,Rightlastfind=0;//��һ���Ƿ��ҵ��˱���
uint8 Leftlast,Rightlast;
uint8 RoadType=0;
int leftfind,rightfind;//flag��ʾ��ǰ�ҵ��Ŀ����Ǳ���
int Right=127,Left=0;//�����㷨�ҵ������ұ��ߵ�ֵ
int CCD_Diff[128];//���ֵ����
float Middle_Err=0,Middle_Last=0;//����ƫ����
float Correct_Err=0,K_Barraicade=0.5;;
//�����������ͽ�������Կ���
void RoadType_Distinguish(void){
  EdgeFind();
  /*
  switch(RoadType){
    case 0:
      Normal(); //��ͨ����
      break;
    case 1:
      CrossRoad();  //ʮ��·
      break;
    default:
      Normal();  //��ͨ����
      break;
  
  }*/
  Normal();

  if(RoadType==1){
  
    LPLD_GPIO_Output_b(PTE, 10, 0);
  
  }
  
  if(Right-Left <10){ //��ʱ���ұ�����Ȼ�ҵ���ȴ������
    Right = Rightlast;
    Left = Leftlast;
  
  }
  
    //��������Ϣд�뷢������
  CCD_Draw_Buff[128]=Left-1;
  CCD_Draw_Buff[129]=Right; 
  CCD_Draw_Buff[132]=BYTE0(Max_Peak);
  CCD_Draw_Buff[133]=BYTE1(Max_Peak);
  CCD_Draw_Buff[134]=Leftlastfind;
  CCD_Draw_Buff[135]=Rightlastfind;
  CCD_Draw_Buff[136]=RoadType; 
  Leftlast=Left;              // ���±߽���
  Rightlast=Right;
  if(Rightlastfind||Leftlastfind)//�������������ҵ�����һ��
  {
    Middle_Err=64-(Leftlast+Rightlast)/2.0+1;//����õ�����ƫ����
    if(ABS(Middle_Err-Middle_Last)>2) 
    {
      if(ABS(Middle_Err-Middle_Last)>3)  //���ܳ��ִ���ɼ���������ƫ��������΢��
      {
        if(Middle_Err>Middle_Last) Middle_Err=Middle_Last+1;
        else Middle_Err=Middle_Last-1;
      }
      else  //�ں���ɼ���Χ�ڣ�������ƫ�������и���
      {
        Middle_Err=(Middle_Last+Middle_Err)/2;
        
      }
    
    }
    Middle_Last=Middle_Err;
  }



}
void CrossRoad(void){
  if(Rightlastfind==0 && Leftlastfind==0){ //����ʮ��
    
    
    
  } 


}

//�Գ��������Ĵ���
void Normal(void){

  if(Leftlastfind==0&&Rightlastfind==0){  //�����һ���������߾�δ�ҵ�
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
        if(Right-Left<10)
        {
         Leftlastfind=0;
         Rightlastfind=0;
        }
       }    
    }
  
  }else{
    if(Leftlastfind){//����ϴ��ҵ�������� 
      if(FindNearest(LEFT,Leftlast)){
        Left=RisingEdge[RisingEdgeIndex];
        Leftlastfind=1;	
        if(Rightlastfind==0){ //�ϴ���ξ��ҵ�������ߣ��ϴ��Ҳ����ұ���
          if(FallingEdgeCnt){
            for(FallingEdgeIndex=0;FallingEdgeIndex<FallingEdgeCnt;FallingEdgeIndex++){
              if(FallingEdge[FallingEdgeIndex]>Left) {            
                if(FallingEdge[FallingEdgeIndex]>75 && FallingEdge[FallingEdgeIndex]< 90){
                  Right=FallingEdge[FallingEdgeIndex];
                  Rightlastfind=1;
                  break;
                }
                
              }
            }
            
            if(Rightlastfind==0){  //��ʱ���ߣ������㷨����
              //Right=Left+Reference_Width;
              Right = 88;

            }
          
          }
          else{  //ֱ��û���½��أ����ߣ����в���
          
            Right = 88;
          }
          
          
        
        }
      
      }else{
        Leftlastfind=0;
        if(Find_Boundary(LEFT)){
          if(RisingEdge[RisingEdgeIndex]<Leftlast){
            if(Rightlastfind==0){
              RoadType=1; //����ʮ�ֺ������б��
            }
          }
        
        
        }else {  //������߽綼û�ҵ�
          if(Rightlastfind==0){  //�϶��ǽ���ʮ����
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

                if(RisingEdge[RisingEdgeIndex]<Right){

                   if(RisingEdge[RisingEdgeIndex]>42 && RisingEdge[RisingEdgeIndex]< 80){
                      Left=RisingEdge[RisingEdgeIndex];
                      Leftlastfind=1;
                      break;
                  }
                  
                  
                
                }
             
             }
             if(Leftlastfind==0){
             

               Left = 42;

             }

           }
           else{

              Left = 42;
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
  
  

  
  
  }
  


}


//Ѱ��������,�߽��߿��ܳ�������Щ��������
void EdgeFind(void){
  int i = 0;//for ���±�
  //�Ը�ֵ���г�ʼ��
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

    if(ABS(CCD_Diff[i])>Max_Peak) Max_Peak=ABS(CCD_Diff[i]);//Ѱ�Ҳ��ֵ�ķ�ֵ 
  }
  for(i=4;i<127;i++)
  {
    if((CCD_Diff[i]>=CCD_Diff[i-1])&&(CCD_Diff[i]>CCD_Diff[i+1])&&(CCD_Diff[i]>Threshold)) //Ѱ�����ķ�ֵ�������
    {
      if(RisingEdgeCnt<5)    
      { 
      
         RisingEdge[RisingEdgeCnt]=i;
         RisingEdgeCnt++;
     
        
      }
    } 
    if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold))  //Ѱ�Ҹ��ķ�ֵ���ұ���
    {
      if(FallingEdgeCnt<5)    
      {
        
            FallingEdge[FallingEdgeCnt]=i;
            FallingEdgeCnt++;
     

      }
    }
  }

}


//ͬʱѰ����������
void Find_BothLine(void){       
  uint8 i=0,j=0,find=0;//find���Ƿ�ͬʱ�ҵ������ұ���
  for(j=55;j>35;j--)
  {
    if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) //Ѱ�����ķ�ֵ
    {
      break;
    }
  }
  for(i=80;i<100;i++)
  {
    if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold))  //Ѱ�Ҹ��ķ�ֵ
    {
      break;
    }
  }
  if(j>42&&i<89)
  {
    find=1;
    Left=j;
    Right=i;
    Leftlastfind=1;
    Rightlastfind=1;  
  }
  else
  {    /*
    //�Ѹ���ȥ��֮�󣬸о�else�����Ҳûɶ�ã�û�����߹̶����Ʊ�����
    for(j=45;j<80;j++)         
    {
       if((CCD_Diff[j]>=CCD_Diff[j-1])&&(CCD_Diff[j]>CCD_Diff[j+1])&&(CCD_Diff[j]>Threshold)) 
      {
        for(i=j+1;i<=90;i++) 
        {
          if((CCD_Diff[i]<CCD_Diff[i-1])&&(CCD_Diff[i]<=CCD_Diff[i+1])&&(CCD_Diff[i]<-Threshold)) 
          {
            if(i-j>15)
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
    } */
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
         if(RisingEdge[RisingEdgeIndex]<80 && RisingEdge[RisingEdgeIndex] > 42 )
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
      if(FallingEdge[FallingEdgeIndex]>53  && FallingEdge[FallingEdgeIndex]< 89)
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
       if(ABS(lastedge-RisingEdge[RisingEdgeIndex])<=RANGE_NEAR  && RisingEdge[RisingEdgeIndex]<80 && RisingEdge[RisingEdgeIndex] > 40)
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
       if(ABS(lastedge-FallingEdge[FallingEdgeIndex])<=RANGE_NEAR  && FallingEdge[FallingEdgeIndex]>53  && FallingEdge[FallingEdgeIndex]< 89)
       {
         find=1;
         break;
       }
     }
    }  
  
  }

  return find;
}


