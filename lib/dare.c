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
#include "dare.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct exception_line_st {
	char const *str;
	struct exception_line_st *above;
	struct exception_line_st *below;
};

struct exception_st {
	char const *msg;
	int code;
	struct exception_st *cause;
	struct exception_line_st *top;
	struct exception_line_st *bottom;
};

char const * get_msg(Exception e) {
	if (!e) return NULL;
	return e->msg;
}

int get_code(Exception e) {
	if (!e) return 0;
	return e->code;
}

Exception get_cause(Exception e) {
	if (!e) return NULL;
	return e->cause;
}

void fprint_stacktrace(FILE *fp, Exception e) {
	if (!e || !fp) return;
	
	fprintf(fp, "Exception: (%d) %s\n", e->code, e->msg);
	struct exception_line_st *line = e->bottom;
	while (line) {
		puts(line->str);
		line = line->above;
	}

	while ((e = e->cause)) {
		fprintf(fp, "Caused by: (%d) %s\n", e->code, e->msg);
		struct exception_line_st *line = e->bottom;
		while (line) {
			puts(line->str);
			line = line->above;
		}
	}
}

void print_stacktrace(Exception e) {
	fprint_stacktrace(stdout, e);
}

Exception new_exception(char const *msg, int code, Exception cause) {
	if (!msg) return NULL;

	Exception e = malloc(sizeof *e);
	if (!e) return NULL;

	e->msg = msg;
	e->code = code;
	e->cause = cause;
	e->top = NULL;
	e->bottom = NULL;
	return e;
}

Exception add_line(Exception e, char const *str) {
	if (!e) return NULL;

	struct exception_line_st *line = malloc(sizeof *line);
	if (!line) return NULL;

	line->str = str;
	line->above = NULL;
	line->below = e->top;
	e->top = line;
	if (line->below)
		line->below->above = line;
	else
		e->bottom = line;

	return e;
}

void cancel(Exception e) {
	if (!e) return;
	while (e->top) {
		struct exception_line_st *garbage = e->top;
		e->top = e->top->below;
		free(garbage);
	}
	free(e);
}