/**
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "int_stack.h"

#define INT_STACK_MIN 256


void init_int_stack(int_stack_t *stack)
{
    stack->size = INT_STACK_MIN;
    stack->top = 0;
    stack->data = malloc(sizeof(uint32_t) * INT_STACK_MIN);
}


void delete_int_stack(int_stack_t *stack)
{
    free(stack->data);
}


uint32_t int_stack_top(int_stack_t *stack)
{
    return stack->data[stack->top];
}


void int_stack_extend(int_stack_t *stack, uint32_t size)
{
    assert(stack->size < INT_STACK_MAX);
    size_t new_size = stack->size + size;
    stack->size = new_size;
    stack->data = realloc(stack->data, sizeof(uint32_t) * new_size);
}


void int_stack_push(int_stack_t *stack, uint32_t state)
{
    if (stack->top == stack->size) {
        int_stack_extend(stack, stack->size / 2);
    }
    stack->data[stack->top] = state;
    stack->top++;
}


uint32_t int_stack_pop(int_stack_t *stack)
{
    assert(stack->top != 0);
    uint32_t state = stack->data[stack->top-1];
    stack->top--;
    return state;
}