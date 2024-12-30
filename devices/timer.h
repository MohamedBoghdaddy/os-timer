#ifndef DEVICES_TIMER_H
#define DEVICES_TIMER_H


#include <stdint.h>

void timer_init(void);
void timer_sleep(int64_t ticks);
int64_t timer_ticks(void);
int64_t timer_elapsed(int64_t then);

#endif 
