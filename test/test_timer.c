#include <stdio.h>
#include "../devices/timer.h"

void test_timer_sleep(void) {
    printf("Test: Sleeping for 100 ticks...\n");
    timer_sleep(100);
    printf("Test: Woke up after 100 ticks.\n");
}

void run_alarm_clock_tests(void) {
    timer_init();
    test_timer_sleep();
}
