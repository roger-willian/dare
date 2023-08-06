# Example code

This directory contains example code demonstrating how to include de dare exception handling library in a simple application.
To build the example, issue the command `make` in this directory. (Assuming you have the proper tools installed.)
That should build a binary called `calc` which is a simple Reverse Polish Notation (RPN) calculator, i.e. a stack based calculator.

To use the calculator you must input the operands (numbers) and push them into the stack before doing the operations.
For example, to add two and two you should input `2` then `2` then `+`. It should be straightforward.

To test the exception handling mechanism, try entering `2` only once and then `+`, for example.
This should throw an exception since you don't have enough operands in the stack to perform the sum.

Examples of exceptions that you may try to raise in the calculator program:

- Stack full;
- Stack empty;
- Not enough operands;
- Division by zero;
- Square root of negative number;
- Invalid operation.

Immediately upon start, the calculator should present the valid commands on the screen. If you need them presented again, just input `help`. When you want to terminate the calculator, simply input `quit`.
