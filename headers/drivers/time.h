#include "../idt.h";
#pragma once
int ticks = 0;
int seconds = 0;
void timer_handler(registers *r)
{
	ticks++;
	if (ticks % 18 == 0)
	{
		seconds++;
	}
    return;
}

void wait_t(int tick_c){
    int start = ticks;
    while(ticks != start + tick_c);
    return;
}

void waint_s(int sec_c){
    int start = seconds;
    while(seconds != start + sec_c);
    return;
}