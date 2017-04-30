#ifndef _SEARCH_H_
#define _SEARCH_H_
#include "common.h"
extern uint8 CCD_Draw_Buff[150];
extern uint16 CCD_Buff[128];
void RoadType_Distinguish(void);
void EdgeFind(void);
void CrossRoad(void);
void Find_BothLine(void);
void Normal(void);
uint8 Find_Boundary(uint8 mode);
uint8 FindNearest(uint8 mode, uint8 lastedge);
#endif