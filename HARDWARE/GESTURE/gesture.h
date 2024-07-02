#ifndef _GESTYRE_H
#define _GESTYRE_H
#include "init.h"
#include "sys.h"
#include "FDC2X14.h"

extern u8 Detect(int , int);
extern int lastValue[CHNUM];
extern int tempValue[CHNUM];
extern int JumpRange[CHNUM];


#endif
