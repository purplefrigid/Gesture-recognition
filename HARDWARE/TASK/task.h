#ifndef __TASK_H
#define __TASK_H	 
#include "sys.h"
#define TrainLimit 3
struct _TimeMS{
	u8 Count500us;
	u8 Count1ms;
	u8 Count2ms;
	u8 Count4ms;
	u8 Count10ms;
  u8 Count20ms;
  u8 Count50ms;  
	u8 Count100ms;
	u8 Count200ms;
  u8 Count500ms;    
	u8 Count1000ms;
};

extern struct _TimeMS TimeMs;   

void TaskManage(u8 lanya);

extern void Task1(void);
extern void Task2(void);
extern void Task3(void);
extern void Task4(void);

#endif


