//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

static __IO uint32_t msTicks;

//	Declare in main.c
extern kick_struct kick;

void DelayMs(int ms)
{
  msTicks=ms;
  while(msTicks);
}
void SysTick_Handler(void)
{
  if (msTicks!=0)
  {
    msTicks--;
  }
}
void one_time_delay(int duration)
{
	var_clear((1<<DELAY_FLAG));
	if (kick.one_time_flag != 0)
	{
    DelayMs(duration);
    kick.one_time_flag = false;
  }
}
