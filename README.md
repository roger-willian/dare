# Dare Exception Handling Library

Dare is a lightweight C library to emulate exception handling functionality that is present in other high level languages such as Java, for example.
Although C is a very powerful language, the lack of exception handling forces this kind of code:

~~~ c
int foo() {
	int err = do_something();
	if (err) {
		log_err("An error %d occurred", err);
		goto catch;
	}

	err = do_something_else();
	if (err) {
		log_err("Some other error %d occurred", err);
	}

	// etc...
	goto finally;

catch:
	// treat problems here

finally:
	// free() some stuff

	return err;
}
~~~

The code above has at least the following disadvantages:

- It is verbose. This clutters its true intent. It is hard to separate the code related to the functionality from the code related to the error handling and it is easy to miss something important.
- It is repetitive. This invites copy and paste programming and it is error prone.
- It is boring. This makes the devs more prone to avoid writing code to handle errors that they judge to improbable to happen.

This library's main goal is to mitigate those problems:

- Dare is succinct. A single macro like `check()` handles the three basic tasks that repeat often: it detects an exceptional behavior, then it saves the necessary information, and jumps to the exception handling block. This also makes the true intent of the functionality's code more evident.
- Dare is less repetitive. Because of the previous point, there are less lines of code to copy and paste.
- Dare is fun. Ok, may be not that much, but it is easier on the eyes. Besides, if you know that your exception handling code is free of bugs, you can enjoy your life better.

## How to...

This section shows how to do many things using Dare. There is also an example code showing how to use some features of Dare in the folder `example`.

### Throw and catch an Exception

Suppose you have the following data structure representing a fraction and a function to build it. To prevent a division by zero you could write some code like this:

~~~ c
#include <stdio.h>

#define ZERO_DIV_CODE 0
#define ZERO_DIV_MSG "Division by zero"

typedef struct {
	int num;
	int den;
} Fraction;

Fraction fraction_new(int num, int den) {
	Fraction ret = {0};
	try (
		if (den == 0)
		throw(ZERO_DIV_CODE, ZERO_DIV_MSG)

		ret.num = num;
		ret.den = den;
	) catch (
		puts(exception_msg(EVAR));
	)
	return ret;
}

~~~

`try`, `catch`, and `throw` are just macros and the code above expands to:

~~~ c
#include <stdio.h>

#define ZERO_DIV_CODE 0
#define ZERO_DIV_MSG "Division by zero"

typedef struct {
	int num;
	int den;
} Fraction;

Fraction fraction_new(int num, int den) {
	Fraction ret = {0};
	Exception dare_exception = NULL;
	if (den == 0) {
		dare_exception = new_exception("Division by zero", 0, NULL);
		dare_exception = add_line(dare_exception, "  at " __FILE__ ":" xstr(__LINE__)); \
		goto dare_failure;
	}

	ret.num = num;
	ret.den = den;
	goto dare_success;

dare_failure:
	puts(exception_msg(dare_exception));

dare_success:
	return ret;
}
~~~

## Rethrow and catch

I hope it's visible that the code is clearer and scales better (in terms of writing) using the Dare library macros than writing all the goto's and labels by hand.
However, oftentimes you do not want to throw and catch the exception in the same function.
For those times, there are two other macros: `check` and `check_cause`.
The code below illustrates how the `check` macro works:

~~~ c
#include <stdio.h>

#define ZERO_DIV_CODE 0
#define NULL_POINTER_CODE 1
#define ZERO_DIV_MSG "Division by zero"
#define NULL_POINTER_MSG "Tried to access a null pointer"

typedef struct {
	int num;
	int den;
} Fraction;

Exception fraction_new(Fraction *self, int num, int den) {
	try (
		if (!self)
			throw(NULL_POINTER, NULL_POINTER_MSG)
		if (den == 0)
			throw(ZERO_DIV_CODE, ZERO_DIV_MSG)

		self->num = num;
		self->den = den;
		return SUCCESS;
	) catch (
		return EVAR;
	)
}

void foo(int num, int den) {
	try (
		Fraction f;
		check(fraction_new(&f, num, den))
		printf("New fraction is: %d/%d\n", f.num, f.den);
	) catch (
		puts(exception_msg(EVAR));
	)
}
~~~

Now there's a lot going on there, so let me explain it by parts.
The first thing to be noticed is that function `fraction_new()` may throw exceptions now.
This is represented by its return type, which is now `Exception`.
Because of that, it must have an in/out parameter so it can still "return" a `Fraction`.
All functions that need to return a value and throw exceptions will need to follow this pattern, that is, returning an `Exception` and having one or more output parameters.

Next thing we notice is that the `Exception` must be caught inside the function and returned if we expect it to be seen outside the scope of the function.
In other words, there should always be a `try/catch` surrounding the `throw` macro that generates the exception, even if it is only to return the `Exception` caught.

The last thing that should be noticed is that we use the macro `check` around the command that invokes the function that throws the exception in the first place.

Perhaps all those things are better understood by noting that the line containing the macro `check`:

~~~ c
check(fraction_new(&f, num, den))
~~~

simply expands to:

~~~ c
{ \
	dare_exception = fraction_new(&f, num, den); \
	if (dare_exception != SUCCESS) { \
		dare_exception = add_line(dare_exception, "  at " __FILE__ ":" xstr(__LINE__)); \
		goto dare_failure; \
	} \
}
~~~

In other words, the macro `check(expression)` evaluates the expression in parenthesis and treats it as an `Exception` if it is not SUCCESS.
In which case, the current line is included in its stacktrace and the program execution continues at the catch block.

## Rethrow with cause

Sometimes we want to catch an `Exception` but rethrow a different one.
For those cases the Dare library provides the macro `check_cause`.
An example of use is like this:

~~~ c
#include <stdio.h>

#define ZERO_DIV_CODE 0
#define NULL_POINTER_CODE 1
#define FR_CREAT_CODE 37
#define ZERO_DIV_MSG "Division by zero"
#define NULL_POINTER_MSG "Tried to access a null pointer"
#define FR_CREAT_MSG "Fraction creation exception"

typedef struct {
	int num;
	int den;
} Fraction;

Exception fraction_new(Fraction *self, int num, int den) {
	try (
	if (!self)
		throw(NULL_POINTER, NULL_POINTER_MSG)
	if (den == 0)
		throw(ZERO_DIV_CODE, ZERO_DIV_MSG)

	self->num = num;
	self->den = den;
		return SUCCESS;
	) catch (
		return EVAR;
	)
}

void foo(int num, int den) {
	try (
		Fraction f;
		check_cause(fraction_new(&f, num, den), FR_CREAT_CODE, FR_CREAT_MSG)
		printf("New fraction is: %d/%d\n", f.num, f.den);
	) catch (
		puts(exception_msg(EVAR));
	)
}
~~~

This code is very similar to the one before, except that the macro `check_cause` receives three arguments.
What changed is that now if `foo()` is called with 0 in its second argument, for example, there will be two `Exceptions`: the outmost exception and its cause.

When using this pattern, the `check_cause` macro will add any inner `Exception` returned by the first argument as the cause of the outer `Exception`, whose code and message are passed in the other two arguments.
This way we can have as many levels of exceptions as we need.
Each level will have its own stacktrace too.

The example above is very simple, so the stacktrace printed will be something like this:

Exception: (37) Fraction creation exception
	at file.c: 33
Caused by: (0) Division by zero
	at file.c: 20

## Verify some condition

When we need to verify some condition we use assertions.
The Dare library provides some often used assertions.
For example, in the codes above, instead of writing

~~~ c
if (!self)
	throw(NULL_POINTER, NULL_POINTER_MSG)
if (den == 0)
	throw(ZERO_DIV_CODE, ZERO_DIV_MSG)
~~~

we could simply write

~~~ c
assert_not_null(self, NULL_POINTER, NULL_POINTER_MSG)
assert_not_equal(den, 0, ZERO_DIV_CODE, ZERO_DIV_MSG)
~~~

to the same effect.
That is, the first one throws an exception if `self` is `NULL`, the second one throws an exception if the value of `den` is zero.
The code and message of the exception thrown are given in the other two arguments.

Each assertion macro provided does exactly what its name says.
All the assertions provided are listed below:

~~~ c
assert_true(X, MSG, CODE)
assert_false(X, MSG, CODE)
assert_null(X, MSG, CODE)
assert_not_null(X, MSG, CODE)
assert_equal(X, Y, MSG, CODE)
assert_not_equal(X, Y, MSG, CODE)
assert_lt(X, Y, MSG, CODE)
assert_gt(X, Y, MSG, CODE)
assert_le(X, Y, MSG, CODE)
assert_ge(X, Y, MSG, CODE)
assert_str_equal(X, Y, MSG, CODE)
assert_str_not_equal(X, Y, MSG, CODE)
~~~

# Possible problems

If you run into some compilation or runtime problem check the following points:

- Do not use semicolon after the `check` and `check_cause` macros if you are using them inside an `if` clause without braces and before an `else` clause or some similar construction;
- Do not use more than one `try`-`catch` structure for each function;
- Do not use `throw`, `check`, `check_cause` and assertion macros outside the `try` block;
- Avoid calling the internal functions of the library directly, except the ones intended for this type of use;
- Verify that the functions have the correct arguments/parameter and return values.
