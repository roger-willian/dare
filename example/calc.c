#include "dare.h"
#include "stack.h"
#include "tokenizer.h"
#include "engine.h"

// possible main exceptions
#define MAIN_EXCEPTION 2000

#define SYNTAX_ERROR "Syntax error!"

void main_loop(Stack *s) {
  try (
    Token tk = get_token();
    cls();
    switch (tk.t) {
      case OPERATION:
        check(operate(s, tk.value.op));
        break;
      case NUMBER:
        check(number(s,tk.value.num));
        break;
      default:
        throw(SYNTAX_ERROR, MAIN_EXCEPTION);
        break;
    }
  ) catch (
    // puts(get_msg(EVAR));
    print_stacktrace(EVAR);
    // cancel(EVAR);
  )
}

// main program
int main() {
  Stack s = new_stack();

  cls();
  operate(&s, HELP);
  update(s);
  for (;;) {
    main_loop(&s);
    update(s);
  }
}