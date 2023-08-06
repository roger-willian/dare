#ifndef ENGINE_H
#define ENGINE_H
#include "stack.h"
#include "tokenizer.h"
#include "dare.h"

// stack levels to print
#define MAX_LINES 7

// "clean screen" printing 100 blank lines
void cls();

// push a number into the stack's TOP
Exception number(Stack *s, Elem num);

// execute an operation
Exception operate(Stack *s, Operation op);

// print the valid commands
void help();

// update the screen with the stack contents
// and a prompt for the user to enter something
void update(Stack s);

#endif