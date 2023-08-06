#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "stack.h"

//! token max allowed size including terminator
#define TK_MAX 256

//! operation codes
typedef enum {
  SUM,      //< addition
  SUBTRACT, //< subtraction
  MULTIPLY, //< multiplication
  DIVIDE,   //< division
  CLEAR,    //< clear stack
  HELP,     //< list operations
  QUIT,     //< quit program
  SQRT,     //< square root
  INVALID_OP  //< invalid operation
} Operation;

//! token types
typedef enum {
  OPERATION,  //< operation, like: +, -, etc.
  NUMBER,     //< number, like: 10, -0.1, 1.2e3, etc
  INVALID     //< some invalid syntax
} Type;

//! struct holding a token
typedef struct {
  Type t;           //< the token type, i.e. an operation or a number
  union {
    Operation op;   //< the token as operation
    Elem num;       //< the token as number
  } value;          //< the token value
} Token;

//! read a token from the keyboard
Token get_token();

//! return the valid commands
const char **valid_ops();

#endif