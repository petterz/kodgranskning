#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include "utils.h"

#define UPPER_LIMIT 255

typedef struct item item_t;

struct item {
  char * name;
  char *desc;
  int price;
  char *shelf;
};

void print_item(item_t *item){
  int kr = item->price/100;
  int ore = item->price%100;
  printf("Name: %s\n", item->name);
  printf("Desc: %s\n", item->desc);
  printf("Price: %d.%d\n", kr, ore);
  printf("Shelf: %s\n", item->shelf);
}

item_t make_item(char *name, char *desc, int price, char* shelf){
  item_t item = { .name = name, .desc = desc, .price = price, .shelf = shelf };
  return item;
}

bool is_shelf(char *shelf){
  int len = strlen(shelf);
  if(!isalpha(shelf[0])){
      return false;
    }
    for(int i = 1; i < len; ++i){
      if(!isdigit(shelf[i])){
          return false;
        }
    }
    return true;
}

char *ask_question_shelf(char *question){
  return ask_question(question, is_shelf, (convert_func) strdup).s;    
}

item_t input_item(){
  char *name = ask_question_string("What is the name of the product?");
  char *desc = ask_question_string("Please describe this product.");
  int price = ask_question_int("How much does the product cost?");
  char *shelf = ask_question_shelf("At what shelf is the product located?");
  item_t item = make_item(name, desc, price, shelf);
  return item;
}

char *magick(char *ar1[], char *ar2[], char *ar3[], int length){
  char buf[UPPER_LIMIT];
  int t1 = rand() % length;
  int t2 = rand() % length;
  int t3 = rand() % length;
  strcpy(buf, ar1[t1]);
  strcat(buf, "-");
  strcat(buf, ar2[t2]);
  strcat(buf, " ");
  strcat(buf, ar3[t3]);
  strcat(buf, "\0");
  return strdup(buf);
}

void list_db(item_t *items, int no_items){
  puts("=============");
  for(int i = 0; i < no_items; ++i){
    printf("%d. %s\n", i+1, items[i].name);
  }
  puts("============");
}

void edit_db(item_t *items, int no_items){
  int choice;
  do{
    choice = ask_question_int("Which entry would you like to edit?");
  }while(!(choice > 0 && choice <= no_items));
  print_item(&items[choice-1]);
  puts("Input the information for the new product.\n");
  item_t new_item = input_item();
  items[choice-1] = new_item;
}

void print_menu(){
  printf("%s%s%s%s%s%s"                 ,
         "[L]ägga till en vara\n"       ,
         "[T]a bort en vara\n"          ,
         "[R]edigera en vara\n"         ,
         "Ån[g]ra senaste ändringen\n"  ,
         "Lista [h]ela varukatalogen\n" ,
         "[A]vsluta\n");
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
  print_menu();
  char c;
  do{
    c = getchar();
  }while(!menu_check(c));
  char new_c = toupper(c);
  return new_c;
}

void add_item_to_db(item_t *items, int *no_items){ 
  puts("Please input the information about the new product you would like to add");

  item_t new_item;
  new_item = input_item();

  items[*no_items] = new_item;
  ++*no_items;
}

void remove_item_from_db(item_t *items, int *no_items){
  list_db(items, *no_items);
  int choice;

  do{
    choice = ask_question_int("Which entry would you like to remove?");
  }while(!(choice > 0 && choice <= *no_items));

  --*no_items;
  int start = --choice;

  for (; start < *no_items; ++start){
    items[start] = items[start+1];
  }
}

void event_loop(item_t *items, int *no_items){
  char choice;  
  do{
    if((choice == 'L')){
      add_item_to_db(items, no_items);
    }else if((choice == 'T')){
      remove_item_from_db(items, no_items);
    }else if((choice == 'R')){
      edit_db(items, *no_items);
    }else if((choice == 'G')){
      printf("Not yet implemented!\n");
    }else if((choice == 'H')){
      list_db(items, *no_items);
    }
  }
  while((choice = ask_question_menu()) != 'A');
  exit(0);  

  }
  


int main(int argc, char *argv[]){
  char *array1[] = {"Maräng",  "Choklad", "Underbart" }; /// Lägg till!
  char *array2[] = {"svängig", "konstig", "finurlig"  }; /// Lägg till!
  char *array3[] = {"pinne",   "boll",    "läxa"      }; /// Lägg till!

  if (argc < 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    item_t db[16]; // Array med plats för 16 varor
    int db_siz = 0;    // Antalet varor i arrayen just nu

    int items = atoi(argv[1]); // Antalet varor som skall skapas

    if (items > 0 && items <= 16)
    {
      for (int i = 0; i < items; ++i)
      {
        // Läs in en vara, lägg till den i arrayen, öka storleksräknar$
        item_t item = input_item();
        db[db_siz] = item;
        ++db_siz;
      }
    }
    else
    {
      puts("Sorry, must have [1-16] items in database.");
      return 1; // Avslutar programmet!
    }

    for (int i = db_siz; i < 16; ++i)
      {
        char *name = magick(array1, array2, array3, 3); /// Lägg til$
        char *desc = magick(array1, array2, array3, 3); /// Lägg til$
        int price = random() % 200000;
        char shelf[] = { random() % ('Z'-'A') + 'A',
                         random() % 10 + '0',
                         random() % 10 + '0',
                         '\0' };
        item_t item = make_item(name, desc, price, shelf);

        db[db_siz] = item;
        ++db_siz;
      }
    //skriv ut innehållet
    
    event_loop(db, &db_siz);

  }
  return 0;
}

