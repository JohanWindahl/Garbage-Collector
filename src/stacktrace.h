/**
 *   \file stacktrace.h
 *   \brief Stacktrace for possible pointers to heap
 */

#ifndef __stacktrace__
#define __stacktrace__


#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <setjmp.h>



////////////////// Functions


/**
 *  Create and return a pointer to bottom of stack
 *
 *  \return  pointer to the bottom of the stack
 */
void *stack_find_bottom();


/**
 *  Create and return a pointer to top of stack
 *
 *  \return  pointer to the top of the stack
 */
void *stack_find_top();


/**
 *  Check if a pointer points to the heap, to make .......TODO
 *
 *  \param p Pointer to be checked  
 */
bool stack_check_pointer(heap_t *h, void *p);// intptr_t *p


/**
 *  Traverse stack and check each adress if it might contain a pointer to the heap
 *
 *
 */
void* stack_trace(heap_t *h, void *current, void *bottom);




#endif
