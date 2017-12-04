#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>
#include "utils.h"

int clear_input_buffer(void){
  int c;
  do
    {
     c = getchar();
    }
  while (c != '\n'&& c != EOF);
  return 0;
}


void print_message(char *message){
  printf("%s%s\n%s"                     ,
         "===========================\n",
         message                        ,
         "===========================\n");
}

bool yes_or_no(){
  puts("[J]a eller [N]ej");
  int c = 0;
  while(c != 106 || c != 110 || c != 74 || c != 78){
    c = getchar();
    if(c == 106 || c == 74){
      puts("Ja");
      puts("-----------------------");
      clear_input_buffer();
      return true;
    }
    if(c == 110 || c == 78){
      puts("Nej");
      puts("------------------------");
      clear_input_buffer();
      return false;
    }
  }
  return false;
}

bool not_empty(char *str)
{
  return strlen(str) > 0;
}


bool is_number(char *str){
  int len = strlen(str);
  for (int i = 0; i < len; ++i){
    if(str[0] == '-' || !isdigit(str[i])){
      return false;
    }
  }
  return true;
}



int read_string(char *buf, int buf_siz){
  int c;
  int i = 0;
  while((c = getchar()) != '\n' && c != EOF){
    if(i < buf_siz){
      buf[i] = c;
      ++i;
    }else{
      clear_input_buffer();
      break;
    }
  }
  buf[i] = '\0';
  return i;
}

void print(char *str){
  for(; *str != '\0'; ++str){
    putchar(*str);
  }
}

void printnl(char* str){
  print(str);
  putchar('\n');
}


//function inspired by Jonathan Leffler @ stackoverflow
bool same_string(char *word1, char *word2){
  if (strlen(word1) != strlen(word2)){
    return false;
  }
  int i;
  int result = 1;
  for (i = 0; word1[i] != '\0' && word2[i] != '\0'; ++i){
    if(word1[i] == word2[i]){
      result = 0;
      break;
    }
  }
  return result;
}


answer_t ask_question(char *question, check_func check, convert_func convert){
  char ans[255];
  while (1){
    printf("%s\n", question);
    printf("-----------------------\n");
    read_string(ans, 255);
    if(check(ans)){
      printf("%s\n", ans);
      printf("-----------------------\n");
      break;
    }
    printf("Felaktig inmatning: '%s'\n\n", ans);
  }
  return convert(ans);
}

int ask_question_int(char *question){
  answer_t answer = ask_question(question, is_number, (convert_func) atoi);
  return answer.i; // svaret som ett heltal
}

char *ask_question_string(char *question){
  char *ans = ask_question(question, not_empty, (convert_func) strdup).s;
  return ans;
}


bool menu_check(char a){
  return(a == 'L' ||
         a == 'l' ||
         a == 'T' ||
         a == 't' ||
         a == 'R' ||
         a == 'r' ||
         a == 'G' ||
         a == 'g' ||
         a == 'H' ||
         a == 'h' ||
         a == 'A' ||
         a == 'a');
}

char ask_question_menu(){
  char c;
  do{
    c = getchar();
  }while(!menu_check(c));
  clear_input_buffer();
  char new_c = toupper(c);
  return new_c;
}

char get_a_char(){
  char c = getchar();
  c = toupper(c);
  clear_input_buffer();
  return c;
}

bool check_character(char choice, char c){
  if(choice == c || choice == (c+32)){
    return true;
  }
  return false;
}

/*
int main(void){
  int buf_siz = 255;
  char buf[buf_siz];
  int num;
  char question_num[] = "Vad är din favoritsiffra?";
  
  int fav = (buf, ask_question_int(question_num));
  return 0;
}

*/
