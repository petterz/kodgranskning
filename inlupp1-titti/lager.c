#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "list.h"
#include "tree.h"

typedef struct action{
  enum {NOTHING, ADD, REMOVE, EDIT} type;
  T merch;
  T copy;
} action_t;

struct item {
  char* desc;
  int price;
  list_t* shelves;
};

void print_menu(){
  puts ("Välkommer till lagerhantering 1.0\n"
        "=================================\n\n"
        "[L]ägga till en vara\n"
        "[T]a  bort en vara\n"
        "[R]edigera en vara\n"
        "Ån[g]ra senaste ändringen\n"
        "Lista [h]ela varukatalogen\n"
        "[A]vsluta\n"
        );
}

int list_index(list_t* list, char* shelf){
  int len = list_length(list);
  for (int i = 0; i < len; i++){
    if (strcmp(list_get(list, i)->name, shelf) == 0)
      return i;
  }
  return -1;
}

T copy_goods(T goods){
  T new_goods = calloc (1, sizeof(struct item));
  new_goods->desc = strdup(goods->desc);
  new_goods->price = goods->price;
  new_goods->shelves = list_new();
  int len = list_length(goods->shelves);
  for (int i = 0; i < len; i++){
    L* elem = list_get(goods->shelves, i);
    L new_elem = {.name = strdup(elem->name), .quantity = elem->quantity};
    list_append(new_goods->shelves, new_elem);
  }
  return new_goods;
}

bool check_goods(tree_t* tree, char* str){
  return tree_has_key(tree, str);
}

char* get_goods_desc(tree_t* tree, char* name){
  return tree_get(tree, name)->desc;
}

int get_goods_price(tree_t* tree, char* name){
  return tree_get(tree, name)->price;
}

void store_goods(tree_t* tree, char* name, char* desc, int price, char* shelf, int quantity){
  L temp_shelf = {.name = shelf, .quantity = quantity};
  if (tree_has_key(tree, name)){
    T elem = tree_get(tree, name);
    int index = list_index(elem->shelves, shelf);
    if (index < 0){
      list_append(elem->shelves, temp_shelf);
    } else {
      L* shelf = list_get(elem->shelves, index);
      shelf->quantity += quantity;
    }
    return;
  }
  list_t* temp_list = list_new();
  list_append(temp_list, temp_shelf);
  T temp_elem = malloc (sizeof (struct item));
  temp_elem->desc = desc;
  temp_elem->price = price;
  temp_elem->shelves = temp_list;
  tree_insert(tree, name, temp_elem);
}

bool check_shelf(tree_t* t, char* shelf){
  T* elements = tree_elements(t);
  int elem_len = tree_size(t);
  for (int i = 0; i < elem_len; i++){
    int list_len = list_length(elements[i]->shelves);
    for (int j = 0; j < list_len; j++){
      if (strcmp (list_get(elements[i]->shelves, j)->name, shelf) == 0){
        free(elements);
        return true;
      }
    }
  }
  return false;
}

bool has_shelf(tree_t* t, char* name, char* shelf){
  T elem = tree_get(t, name);
  if (elem == NULL)
    return false;
  if (elem->shelves == NULL)
    return false;
  int len = list_length(elem->shelves);
  for (int i = 0; i < len; i++){
    if (strcmp(list_get(elem->shelves, i)->name, shelf) == 0)
      return true;
  }
  return false;
}

void add_goods(tree_t* t, action_t* action){
  action->type = ADD;
  char* goods_name = ask_question_string("Namn: ");
  char* goods_desc;
  int goods_price;
  if (check_goods(t, goods_name)){
    printf("%s finns redan i databasen.\n"
           "Använder samma pris och beskrivning\n", goods_name);
    goods_desc = get_goods_desc(t, goods_name);
    goods_price = get_goods_price(t, goods_name);
  } else {
    goods_desc = ask_question_string("Beskrivning: ");
    goods_price = ask_question_int("Pris: ");
  }
  char* goods_shelf;
  do{
  goods_shelf = ask_question_shelf("Lagerplats: ");
  }while (check_shelf(t, goods_shelf) && !has_shelf(t, goods_name, goods_shelf));
  int goods_quantity = ask_question_int("Antal: ");

  store_goods(t, goods_name, goods_desc, goods_price, goods_shelf, goods_quantity);
  
}

void remove_goods(tree_t* t){
  puts("HAH\nNope\n");
}

char* list_goods(tree_t* t){
  int len = tree_size(t);
  if (len == 0){
    return "";
  }
  char* result = "";
  K* keys = tree_keys(t);
  int const items_per_page = 20;
  int index = 0, page = 0;
  while (index+page*items_per_page < len){
    printf("%d: %s\n", index+1, keys[index+page*items_per_page]);
    index++;
    if (index >= items_per_page || index + page*items_per_page == len){
      index = 0;
      bool end = false;
      do{
        puts("\nVälj index\n"
             "[A]vbryt");
        if (((page+1)*items_per_page) < len)
          puts("[N]ästa sida");
        char* input = ask_question_string("");
        if(is_number(input)){
          int input_int = atoi(input);
          if (input_int > 0 && input_int <= items_per_page && input_int <= len-items_per_page*page){
            result = strdup(keys[atoi(input) + page*items_per_page-1]);
            end = true;
            len = 0;
          }
        } else {
          if (strcmp(input, "a") == 0){
            end = true;
            len = 0;
          } else {
            if (strcmp(input, "n") == 0 && ((page+1)*items_per_page) < len){
              end = true;
            } else {
              end = true;
              page--;
            }
          }
        }
      } while (!end);
      
      page++;
    }
  }
  free(keys);
  return result;
}

void display_goods(tree_t* t){
  char* key = list_goods(t);
  if (strcmp (key, "") == 0)
    return;
  T info = tree_get(t, key);
  printf("Namn: %s\n"
         "Beskrivning: %s\n"
         "Pris: %d\n", key, info->desc, info->price);
  int len = list_length(info->shelves);
  for (int i = 0; i < len; i++){
    L* elem = list_get(info->shelves, i);
    printf("Lagerhylla: %s\n"
           "Antal: %d\n", elem->name, elem->quantity);
  }
}

void edit_goods(tree_t* t, action_t* action){
  char* key = list_goods(t);
  if (strcmp (key, "") == 0)
    return;
  T elem = tree_get (t, key);
  T copy = copy_goods(elem);
  int len, shelf;
  bool valid, changed = true;
  puts("[B]eskrivning\n"
       "[P]ris\n"
       "[L]agerhylla\n"
       "An[t]al\n\n");
  do{
    valid = true;
    char input = ask_question_char("Välj rad eller [a]vbryt: ");
    switch(toupper(input)){
    case 'B':
      printf("Nuvarande Beskrivning: %s\n"
             "--------------------------------------------------------\n", elem->desc);
      elem->desc = ask_question_string("Ny beskrivning: ");
      break;
    case 'P':
      printf("Nuvarande pris: %d kr\n"
             "--------------------------------------------------------\n", elem->price);
      elem->price = ask_question_int("Nytt pris: ");
      break;
    case 'L':
    case 'T':
      len = list_length(elem->shelves);
      for (int i = 0; i < len; i++){
        printf("%d: %s\n", i+1, list_get(elem->shelves, i)->name);
      }
      shelf = ask_question_int("Välj lagerhylla: ");
      switch (toupper(input)){
      case 'L':
      printf("Nuvarande lagerhylla: %s\n"
             "--------------------------------------------------------\n", list_get(elem->shelves, shelf-1)->name);
      char* new_shelf = ask_question_string("Ny lagerhylla: ");
      if (has_shelf(t, key, new_shelf)){
        int count = list_get(elem->shelves, shelf-1)->quantity;
        L* x = malloc (sizeof(L));
        list_remove (elem->shelves, shelf-1, x);
        free (x);
        int len = list_length(elem->shelves);
        for (int i = 0; i < len; i++){
          if (strcmp(list_get(elem->shelves, i)->name, new_shelf) == 0){
            list_get(elem->shelves, i)->quantity += count;
          }
        }
      } else if (check_shelf(t, new_shelf)){
        puts("Upptagen lagerhylla\n");
      } else {
        list_get(elem->shelves, shelf-1)->name = new_shelf;
      }
        break;
      case 'T':
      printf("Nuvarande antal: %d\n"
             "--------------------------------------------------------\n", list_get(elem->shelves, shelf-1)->quantity);
      list_get(elem->shelves, shelf-1)->quantity = ask_question_int("Nytt antal: ");
        break;
      }
      break;
    case 'A':
      changed = false;
      break;
    default:
      valid = false;
      break;
    }
  } while (!valid);
  if (changed){
    action->type = EDIT;
    action->merch = elem;
    action->copy = copy;
  }
}

void undo_action(tree_t* tree, action_t* action){
  T* elems;
  int len;
  switch(action->type){
  case NOTHING:
    break;
  case ADD:
    break;
  case EDIT:
    //Find item to edit back
    elems = tree_elements (tree);
    len = tree_size(tree);
    for (int i = 0; i < len; i++){
      if (strcmp (list_first(action->merch->shelves)->name, list_first(elems[i]->shelves)->name) == 0){
        elems[i]->desc = action->copy->desc;
        elems[i]->price = action->copy->price;
        elems[i]->shelves = list_new(); // HOLY FUCKING SHIT FIX THIS DAMN LEAK THE BOAT IS SINKING
        for (int j = 0; j < list_length(action->copy->shelves); j++){
          list_append(elems[i]->shelves, *list_get(action->copy->shelves, j));
        }
      }
    }
    action->type = NOTHING;
    break;
  case REMOVE:
    break;
  }
}

void exit_program(tree_t* t){
  tree_delete(t, NULL);
}

void event_loop(){
  tree_t* storage_tree = tree_new();
  action_t* last_action = calloc (1, sizeof(action_t));
  last_action->type = NOTHING;
  char input;
  do{
    print_menu();
    input = toupper(ask_question_char("Vad vill du göra idag? "));
    switch (input){
    case 'L':
      add_goods(storage_tree, last_action);
      break;
    case 'T':
      //remove_goods(storage_tree);
      break;
    case 'R':
      edit_goods(storage_tree, last_action);
      break;
    case 'G':
      undo_action(storage_tree, last_action);
      break;
    case 'H':
      display_goods(storage_tree);
      break;
    }
  } while(input != 'A');
  exit_program(storage_tree);
}
int main(int argc, char* argv[]){
  event_loop();
  return 0;
}
