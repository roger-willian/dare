/*
MIT License

Copyright (c) 2022-2023 Roger W. P. da Silva

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef DARE_H
#define DARE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//! An struture representing an exception
typedef struct exception_st * Exception;

/*!
 * Construct a new Exception with a message, a code and, possibly, a cause.
 *
 * Try not to call this function directly. Using the macros throw(), check() or
 * check_cause() usually results in a clearer code. If you must use this, do not
 * forget to call add_line() passing the current line as string.
 *
 * \param msg   The message describing the Exception.
 * \param code  An integer code representing the Exception class.
 * \param cause Another optional Exception that caused this new Exception, or
 * NULL, in case there is none.
 * \return      The new Exception created or NULL in case of error.
 */
Exception new_exception(char const *msg, int code, Exception cause);

/*!
 * Add one line to the stacktrace of the given Exception.
 *
 * Try not to to call this function directly, the macros throw(), check() and
 * check_cause() already take care of it.
 *
 * \param e   The Exception to which the line will be added.
 * \param str The line itself, something like "at myfile.c:20".
 * \return    The same Exception with the line added or NULL in case of error.
 */
Exception add_line(Exception e, char const *str);

/*!
 * Extract the message from an Exception.
 *
 * \param e The Exception whose message will be extracted.
 * \return  The message or NULL in case of error.
 */
char const *get_msg(Exception e);

/*!
 * Extract the code from an Exception.
 *
 * \param e The Exception whose code will be extracted.
 * \return  The code or 0 in case of error.
 */
int get_code(Exception e);

/*!
 * Return the Exception that caused this Exception.
 *
 * \param e The caused Exception whose cause is to be extracted.
 * \return The Exception which is the underlying cause, if any, otherwise return
 * NULL.
 */
Exception get_cause(Exception e);

/*!
 * Print the Exception's message and stacktrace.
 *
 * The message and stacktrace are printed formatted to the file pointed by the
 * first argument, including other Exceptions that caused the one passed in the
 * second argument.
 *
 * \param fp The file pointer to which the stacktrace will be printed, a stream,
 * possibly stdout or stderr.
 * \param e The Exception whose stacktrace will be printed.
 */
void fprint_stacktrace(FILE *fp, Exception e);

/*!
 * Print the Exception's stacktrace directly to the stdout.
 *
 * Same as `fprint_stacktrace(stdout, e)`
 *
 * \param e The Exception whose stacktrace will be printed.
 */
void print_stacktrace(Exception e);

/*!
 * Destroy an Exception freeing all its allocated memory.
 *
 * Just call this function after completely treating the Exception. Do not call
 * it before rethrowing, i.e. returning the Exception.
 *
 * \param e The Exception to be destroyed.
 */
void cancel(Exception e);

// Some auxiliary macros
#define xstr(X) str(X)
#define str(X) #X
#define dare_noop ((void)0)

//! Success is indicated by returning a NULL pointer, i.e. no Exception.
#define SUCCESS NULL
//! This is the name of the Exception variable, redefine at will.
#define EVAR dare_exception

/*!
 * This macro defines a try clause that receives a BLOCK of code.
 *
 * It simply declares the Exception variable and initializes it with SUCCESS.
 * Because of this declaration only one try clause is permitted by each scope.
 * I'd go further and recommend only one for each function.
 *
 * \example
 * try (
 *     // some code here that may throw Exceptions
 * ) catch (
 *     // some code here that deals with the Exception at EVAR
 * )
 */
#define try(BLOCK) \
  Exception EVAR = SUCCESS; \
  BLOCK

/*!
 * This macro defines a check clause that receives a call to a function that may
 * throw, i.e. return an Exception.
 *
 * This macro should be used only inside a try block and only around functions
 * that may return Exceptions.
 *
 * \example
 * try (
 *     // suppose division throws if b is zero
 *     check(division(a,b))
 * ) catch (
 *     // deal with the Exception at EVAR
 * )
 */
#define check(EXPR) { \
  EVAR = EXPR; \
  if (EVAR != SUCCESS) { \
    EVAR = add_line(EVAR, "  at " __FILE__ ":" xstr(__LINE__)); \
    goto dare_failure; \
  } \
}

/*!
 * This macro defines a check clause that checks for an Exception, and throws
 * another one with the first one as its cause.
 *
 * This macro should be used only inside a try block and only around functions
 * that may return Exceptions.
 *
 * \example
 * try (
 *     // suppose division throws if b is zero
 *     check_cause(division(a,b), INVALID_OP, "Uh-oh!")
 * ) catch (
 *     // if division throws EVAR has an Exception with cause here
 * )
 */
#define check_cause(EXPR, MSG, CODE) { \
  EVAR = EXPR; \
  if (EVAR != SUCCESS) check(new_exception(MSG, CODE, EVAR)) \
}

/*!
 * This macro defines a catch clause to deal with a possible Exception thrown
 * from the try block before.
 *
 * Because it declares labels it should be only one catch clause per function
 * immediately following the end of the try block.
 *
 * \example
 * try (
 *     // suppose a Exception maybe thrown here
 * ) catch (
 *     // it will be accessible here in EVAR
 *     print_stacktrace(EVAR);
 * )
 */
#define catch(BLOCK) \
  goto dare_success; \
dare_failure: \
  BLOCK \
dare_success: \
  dare_noop;

/*!
 * This macro throws a new Exception with a message and a class code.
 *
 * This macro must be used only inside a try block. If the exception is to be
 * rethrown by the surrounding function, it must be returned from the catch
 * block.
 *
 * \example
 * try (
 *     // throw the Exception here
 *     if (answer != 42) throw("Something is wrong", 42)
 * ) catch (
 *     // catch and rethrow it here
 *     return EVAR;
 * )
 */
#define throw(MSG, CODE) { \
  EVAR = new_exception(MSG, CODE, NULL); \
  EVAR = add_line(EVAR, "  at " __FILE__ ":" xstr(__LINE__)); \
  goto dare_failure; \
}

/*!
 * This macro throws an Exception with message and class code if its argument
 * is zero (not TRUE).
 */
#define assert_true(X, MSG, CODE) { if (!(X)) throw(MSG, CODE) }

/*!
 * This macro throws an Exception with message and class code if its argument
 * is not zero (not FALSE).
 */
#define assert_false(X, MSG, CODE) { if (X) throw(MSG, CODE) }

/*!
 * This macro throws an Exception with message and class code if its argument
 * is not NULL.
 */
#define assert_null(X, MSG, CODE) { assert_true(X == NULL, MSG, CODE) }

/*!
 * This macro throws an Exception with message and class code if its argument
 * is NULL.
 */
#define assert_not_null(X, MSG, CODE) { assert_false(X == NULL, MSG, CODE) }

/*!
 * This macro throws an Exception with message and class code if its arguments
 * are not equal.
 */
#define assert_equal(X, Y, MSG, CODE) { assert_true(X == Y, MSG, CODE) }

/*!
 * This macro throws an Exception with message and class code if its arguments
 * are equal.
 */
#define assert_not_equal(X, Y, MSG, CODE) { assert_false(X == Y, MSG, CODE) }

/*!
 * This macro throws an Exception with message and class code if its first
 * argument is greater than or equal the second.
 */
#define assert_lt(X, Y, MSG, CODE) { assert_true(X < Y, MSG, CODE) }

/*!
 * This macro throws an Exception with message and class code if its first
 * argument is less than or equal the second.
 */
#define assert_gt(X, Y, MSG, CODE) { assert_true(X > Y, MSG, CODE) }

/*!
 * This macro throws an Exception with message and class code if its first
 * argument is greater than the second.
 */
#define assert_le(X, Y, MSG, CODE) { assert_true(X <= Y, MSG, CODE) }

/*!
 * This macro throws an Exception with message and class code if its first
 * argument is less than the second.
 */
#define assert_ge(X, Y, MSG, CODE) { assert_true(X >= Y, MSG, CODE) }

/*!
 * This macro throws an Exception with message and class code if its string
 * arguments are not equal.
 */
#define assert_str_equal(X, Y, MSG, CODE) { \
  assert_false(strcmp(X, Y), MSG, CODE) \
}

/*!
 * This macro throws an Exception with message and class code if its string
 * arguments are equal.
 */
#define assert_str_not_equal(X, Y, MSG, CODE) { \
  assert_true(strcmp(X, Y), MSG, CODE) \
}

#endif
