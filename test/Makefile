LDLIBS := -lm
CFLAGS := -I../lib

.PHONY : main
main: basic_test assertion_test
	./basic_test && ./assertion_test

basic_test.o: basic_test.c cester.h ../lib/dare.h

basic_test: basic_test.o ../lib/dare.o

assertion_test.o: assertion_test.c cester.h ../lib/dare.h

assertion_test: assertion_test.o ../lib/dare.o

.PHONY : clean
clean:
	${RM} *.o ../lib/*.o basic_test assertion_test