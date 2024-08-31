// Standard Includes
#include <stdio.h>
#include <malloc.h>
#include "pico/stdlib.h"

// Custom Includes
// None

uint32_t getTotalHeap(void) 
{
   extern char __StackLimit, __bss_end__;
   
   return &__StackLimit  - &__bss_end__;
}

uint32_t getFreeHeap(void) 
{
   struct mallinfo m = mallinfo();

   return getTotalHeap() - m.uordblks;
}

void print_free_memory(void)
{
    uint32_t total, free, used;
    total = getTotalHeap();
    free = getFreeHeap();
    used = total - free;
    //printf("\nTotal heap: %dB\nFree heap: %dB\nUsed: %dB\n", getTotalHeap(), getFreeHeap(), used);
    printf("\nTotal heap: %.1fKB\nFree heap: %.1fKB\nUsed: %.1fKB\n", ((float)(getTotalHeap() / 1024)), ((float)(getFreeHeap() / 1024)), ((float)(used / 1024)));
}
