#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "FDC2X14.h"	

extern int Calibration[7/*CHNUM*/];
extern u8 ShowFlag;
extern u16 flag1;
extern void TIM3_Int_Init(u16 arr,u16 psc);
extern void TIM4_Int_Init(u16 arr,u16 psc);
extern void Calibrate(void);
 
#endif
