#include <stdio.h>
#include "../devices/timer.h"  // Include the timer header file

/* Test function for the timer sleep functionality */
void test_timer_sleep(void) {
    printf("Test: Sleeping for 100 ticks...\n");
    timer_sleep(100);  // Sleep for 100 ticks
    printf("Test: Woke up after 100 ticks.\n");
}

/* Function to run alarm clock tests */
void run_alarm_clock_tests(void) {
    timer_init();        // Initialize the timer subsystem
    test_timer_sleep();  // Run the timer sleep test
}

/* Main function to execute the tests */
int main(void) {
    printf("Starting alarm clock tests...\n");
    run_alarm_clock_tests();  // Run the tests
    printf("Alarm clock tests completed.\n");
    return 0;
}