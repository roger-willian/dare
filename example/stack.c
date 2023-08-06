#include "stack.h"
#include "dare.h"

//! possible stack exceptions
#define STACK_EXCEPTION 3000                  //< stack exception code
#define STACK_FULL  "The stack is too full!"  //< the stack is too full
#define STACK_EMPTY "The stack is empty!"     //< the stack is empty

//! create a new stack
Stack new_stack() {
  Stack s;
  s.depth = 0;
  return s;
}

//! push a new element into the top
Exception push(Stack *s, Elem e) {
  try (
    assert_lt(s->depth, S_MAX, STACK_FULL, STACK_EXCEPTION);

    s->vet[s->depth] = e;
    s->depth++;
    return SUCCESS;
  ) catch (
    return EVAR;
  )
}

// pop an element from the top
Exception pop(Stack *s, Elem *e) {
  try (
    assert_gt(s->depth, 0, STACK_EMPTY, STACK_EXCEPTION);

    *e = s->vet[s->depth-1];
    s->depth--;
    return SUCCESS;
  ) catch (
    return EVAR;
  )
}