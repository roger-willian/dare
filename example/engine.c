#include "engine.h"
#include "dare.h"
#include "stack.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// possible engine exceptions
#define ENGINE_EXCEPTION 1000
#define TOO_FEW_ARGS 		"Too few arguments!"
#define ZERO_DIV 				"Division by zero!"
#define CPLX_ROOT 			"Complex root!"
#define INVALID_OPER 		"Invalid operation!"
#define INT_ERROR 			"Internal error!"
#define NUMBER_FAILURE	"Number insertion failure"

// considering a stack like this
//
// ...  :
// NEXT :      a
// TOP  :      b
//--------------

// add NEXT to TOP
Exception sum(Stack *s) {
  try(
		Elem x;
		Elem y;
		Elem z;

		assert_not_null(s, INT_ERROR, ENGINE_EXCEPTION);
		assert_gt(s->depth, 1, TOO_FEW_ARGS, ENGINE_EXCEPTION);

    check(pop(s, &x));
		check(pop(s, &y));
		z = x + y;
		check(push(s, z));
		return SUCCESS;
	) catch (
		return EVAR;
	)
}

// subtract TOP from NEXT
Exception subtract(Stack *s) {
  try(
		Elem x;
		Elem y;
		Elem z;

		assert_not_null(s, INT_ERROR, ENGINE_EXCEPTION);
    assert_gt(s->depth, 1, TOO_FEW_ARGS, ENGINE_EXCEPTION);

    check(pop(s, &y));
		check(pop(s, &x));
		z = x - y;
		check(push(s, z));
		return SUCCESS;
	) catch (
		return EVAR;
	)
}

// multiply NEXT by TOP
Exception multiply(Stack *s) {
	try (
		Elem x;
		Elem y;
		Elem z;

		assert_not_null(s, INT_ERROR, ENGINE_EXCEPTION);
		assert_gt(s->depth, 1, TOO_FEW_ARGS, ENGINE_EXCEPTION);

		check(pop(s, &x));
		check(pop(s, &y));
		z = x * y;
		check(push(s, z));
		return SUCCESS;
	) catch (
		return EVAR;
	)
}

// divide NEXT by TOP
Exception divide(Stack *s) {
	try (
		Elem x;
		Elem y;
		Elem z;

		assert_not_null(s, INT_ERROR, ENGINE_EXCEPTION);
		assert_gt(s->depth, 1, TOO_FEW_ARGS, ENGINE_EXCEPTION);

		check(pop(s, &x));
		if (x == 0) {
			check(push(s, x));
			throw(ZERO_DIV, ENGINE_EXCEPTION);
		}
		check(pop(s, &y));

		z = y / x;
		check(push(s, z));
		return SUCCESS;
	) catch (
		return EVAR;
	)
}

// clean the stack
Exception clean(Stack *s) {
	try (
  	Elem x;
		assert_not_null(s, INT_ERROR, ENGINE_EXCEPTION);

		do {
			check(pop(s, &x));
		} while (s->depth > 0);

		return SUCCESS;
	) catch (
		return EVAR;
	)
}

// extract square root of the TOP
Exception square_root(Stack *s) {
	try (
		Elem x;
		Elem z;

		assert_not_null(s, INT_ERROR, ENGINE_EXCEPTION);
		assert_not_equal(s->depth, 0, TOO_FEW_ARGS, ENGINE_EXCEPTION);

    check(pop(s, &x));
    if (x < 0) {
			check(push(s, x));
			throw(CPLX_ROOT, ENGINE_EXCEPTION);
    }

		z = sqrt(x);
		check(push(s, z));
		return SUCCESS;
	) catch (
		return EVAR;
	)
}

// push a number into the stack's TOP
Exception number(Stack *s, Elem num) {
	try (
		assert_not_null(s, INT_ERROR, ENGINE_EXCEPTION);
		
		check_cause(push(s, num), NUMBER_FAILURE, ENGINE_EXCEPTION);
		return SUCCESS;
	) catch (
		return EVAR;
	)
}


// print the stack
void print(Stack s) {
	int i;
	for (i = s.depth - MAX_LINES; i < s.depth; i++) {
			if (i < 0) printf(" :           \n");
			else printf("%d: %15.12g\n", i, s.vet[i]);
	}
	puts("------------------");
}

// execute an operation
Exception operate(Stack *s, Operation op) {
	try (
		assert_not_null(s, INT_ERROR, ENGINE_EXCEPTION);

		if (op == QUIT) {
			puts("Good bye!");
			exit(0);
		}

		switch (op) {
			case SUM:
				check(sum(s));
				break;
			case SUBTRACT:
				check(subtract(s));
				break;
			case MULTIPLY:
				check(multiply(s));
				break;
			case DIVIDE:
				check(divide(s));
				break;
			case SQRT:
				check(square_root(s));
				break;
			case CLEAR:
				check(clean(s));
				break;
			case HELP:
				help();
				break;
			default:
				help();
				throw(INVALID_OPER, ENGINE_EXCEPTION);
				break;
		}
		return SUCCESS;
	) catch (
		return EVAR;
	)
}

// update the screen with the stack contents
// and a prompt for the user to enter something
void update(Stack s) {
  print(s);
  printf("^: ");
}

// print the valid commands
void help() {
  const char **op_str = valid_ops();
  Operation op = INVALID_OP;
  puts("Operations:");
  for (op = 0; op < INVALID_OP; op++) {
    if (op > 0)
      printf(" , ");
    printf("%s", op_str[op]);
  }
  puts("");
}

// "clean screen" printing 100 blank lines
void cls() {
  int i;
  for (i = 0; i < 100; i++)
    puts("");
}
