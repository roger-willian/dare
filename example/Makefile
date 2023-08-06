LDLIBS := -lm
CFLAGS := -I../lib

.PHONY : main
main: calc

calc: calc.o engine.o stack.o tokenizer.o ../lib/dare.o

.PHONY : clean
clean:
	${RM} *.o ../lib/*.o calc