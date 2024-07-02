#ifndef _LCDINFO_H
#define _LCDINFO_H

#include "stm32f10x.h"

extern void LCDShowTrainInfo_1(void);
extern void LCDShowSelectInfo_1(u8 mode);
extern void LCDShowTrainInfo_2(void);
extern void LCDShowSelectInfo_2(u8 mode);

extern void LcdShowData(void);

extern void LCDShowRowingSelectInfo(void);
extern void LCDShowRowingTrainInfo(u8 mode);
extern void LCDShowRowingJudgeInfo(u8 result);

extern void LCDShowMoraSelectInfo(void);
extern void LCDShowMoraTrainInfo(u8 mode);
extern void LCDShowMoraJudgeInfo(u8 result);

#endif

