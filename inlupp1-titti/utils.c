#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

bool not_empty(char* str){
  return string_length(str) > 0;
}

answer_t ask_question(char* question, check_func checkf, convert_func convertf){
  int buf_siz = 256;
  char input [buf_siz];
  do{
    printf("%s",question);
    read_string(input, buf_siz);
    if (!checkf(input))
      printf("Felaktig inmatning: '%s'\n", input);
  } while (!checkf(input));
  return convertf(input);
}

int string_length(char* str){
  int i = 0;
  while (str[i] != '\0'){
    i++;
  }
  return i;
}

void print(char* str){
  for (int i = 0; i < string_length(str); i++){
    putchar(str[i]);
  }
}

void println(char* str){
  print(str);
  putchar('\n');
}

bool is_digit(char c){
  return ('0' <= c && c <= '9') ? true : false;
}
bool is_number (char *str){
  int length = strlen(str);    
  for (int i = 0; i < length; i++){
    if (i == 0 && str[0] == '-' && length > 1)
      continue;
    if (!is_digit(str[i]))
      return false;
  }
  return true;
}

int ask_question_int (char *question){
  answer_t answer = ask_question(question, is_number, (convert_func) atoi);
  return answer.i;
}

int read_string (char *buf, int buf_siz){
  int i = 0;
  do{
    buf[i] = getchar();
    i++;
  } while (buf[i-1] != '\n' && buf[i-1] != EOF && i-1 < buf_siz);
  i--;
  buf [i] = '\0';
  return i;
}

char *ask_question_string(char *question){
  return ask_question(question, not_empty, (convert_func) strdup).s;
}

bool is_char(char* str){
  int len = string_length(str);
  if (len != 1)
    return false;
  return true;
}

char to_char(char* str){
  return *str;
}

char ask_question_char(char* question){
  return ask_question(question, is_char, (convert_func)to_char).c;
}

bool is_shelf(char* str){
  if (string_length(str) <= 1 || str[0] > 'Z' || str[0] < 'A')
    return false;
  for (int i = 1; str[i] != '\0'; i++){
    if (!is_number (&str[i]))
      return false;
  }
  return true;
}

char* ask_question_shelf(char* question){
  return ask_question(question, is_shelf, (convert_func) strdup).s;
}
