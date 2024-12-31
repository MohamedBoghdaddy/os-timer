#include <stdio.h>
#include "devices/timer.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/interrupt.h"

/* Test function for the timer and alarm functionalities. */
void test_timer_sleep(void) {
    printf("Test: Sleeping for 100 ticks...\n");
    timer_sleep(100); // Sleep for 100 ticks.
    printf("Test: Woke up after 100 ticks.\n");
}

/* Main function to initialize and run tests. */
int main(void) {
    printf("Initializing project...\n");

    // Initialize necessary components
    thread_init();      // Initialize thread subsystem
    timer_init();       // Initialize timer subsystem

    printf("Running tests...\n");
    test_timer_sleep(); // Run timer sleep test.

    printf("All tests completed.\n");
    return 0;
}