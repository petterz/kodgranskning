#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>

typedef union {
  int i;
  float f;
  char* s;
  char c;
} answer_t;
typedef bool(*check_func)(char*);
typedef answer_t(*convert_func)(char*);

int read_string(char*, int);
bool is_digit (char);
bool is_number(char*);
bool not_empty(char*);
answer_t ask_question(char*, check_func, convert_func);
int ask_question_int(char*);
char *ask_question_string(char*);
char ask_question_char(char*);
char* ask_question_shelf(char*);
void print(char*);
void println(char*);
int string_length();

#endif
