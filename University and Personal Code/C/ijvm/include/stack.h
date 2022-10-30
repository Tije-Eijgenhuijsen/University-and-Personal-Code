#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <ijvm.h>

typedef struct
{	
	int sp;
	word_t *data;
	int size;
} stack;


void init_stack(stack *current_stack)
{
	current_stack->sp = -1;
	current_stack->data = malloc(sizeof current_stack->data * current_stack->size);
}

void print_stack(stack *current_stack)
{
	fprintf(stderr, "-------------------------------\n");
	int i = current_stack->sp;
	while(i >= 0)
	{
		fprintf(stderr, "Element %i: %#X\n", i, current_stack->data[i]);
		i--;
	}
	fprintf(stderr, "-------------------------------\n");
}

void push(stack *current_stack, word_t element)
{
	current_stack->sp++;
	if(current_stack->sp == current_stack->size)
	{
		current_stack->size = current_stack->size * 2;
		current_stack->data = realloc(current_stack->data, sizeof current_stack->data * current_stack->size);
	}
	current_stack->data[current_stack->sp] = element;
}

word_t pop(stack *current_stack)
{
	word_t buffer = current_stack->data[current_stack->sp];
	current_stack->sp--;
	return buffer;
}

word_t top(stack *current_stack)
{
	return current_stack->data[current_stack->sp];
}

int size(stack *current_stack)
{
	return current_stack->sp + 1;
}

#endif
