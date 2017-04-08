#ifndef _OLED_H_
#define _OLED_H_
//�������

extern uint8  reverse; //��ת��ʾ\

void OLED_WrCmd(unsigned char cmd);

//��ʾһ��6x8��׼ASCII�ַ�
void OLED_P6x8Char(unsigned char ch);
//����
void OLED_Fill(unsigned char bmp_data);
//����������д��һ��6x8��׼ASCII�ַ���
void OLED_P6x8Str(unsigned char x,unsigned char y,char ch[]); 
//��һ��char����ת����3λ��������ʾ 
void OLED_PrintValueC(unsigned char x, unsigned char y, int data);
//������������һ��int����ת����5λ��������ʾ
void OLED_PrintValueI(unsigned char x, unsigned char y, int data);
//��ʾunsigned int��
void OLED_PrintValueFP(unsigned int data, unsigned char num);
//������������һ��float����ת������������5λ��С���ͷ��ŵ����ݲ�������ʾ
void OLED_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);
//д���ݺ���
void OLED_WrDat(unsigned char data,unsigned char revs);
//������ʾ����λ��
void OLED_Set_Pos(unsigned char x, unsigned char y);
//OLED��ʼ��
void OLED_Init(void);

 void showimage(unsigned char *image);

#endif