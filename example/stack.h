#ifndef STACK_H
#define STACK_H

#include "dare.h"

// max stack size
#define S_MAX 10

// stack elements type
typedef double Elem;

// stack definition
typedef struct {
  int depth;
  Elem vet[S_MAX];
} Stack;

// create a new stack
Stack new_stack();

// push an element into the stack's top
Exception push(Stack *s, Elem e);

// pop an element from the stack's top
Exception pop(Stack *s, Elem *e);

#endif