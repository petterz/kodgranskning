#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include "tree.h"
#include "list.h"
#include "utils.h"

#define UPPER_LIMIT 255

typedef struct shelf {
  char *shelf;
  int amount;
}shelf_t;


typedef struct item {
  char * name;
  char *desc;
  int price;
  list_t *shelfs;
}item_t;

typedef struct action
{
  int type; // NOTHING = 0, ADD = 1, REMOVE = 2, EDIT = 3
  item_t *merch; // pekar på tillagd vara och redigerade
  item_t copy; // sparar item vid borttagning och redigering
}action_t;


void print_menu(){
  printf("%s%s%s%s%s%s%s%s"             ,
         "===========================\n",
         "[L]ägga till en vara\n"       ,
         "[T]a bort en vara\n"          ,
         "[R]edigera en vara\n"         ,
         "Ån[g]ra senaste ändringen\n"  ,
         "Lista [h]ela varukatalogen\n" ,
         "[A]vsluta\n"                  ,
         "===========================\n"
         );
}



bool is_shelf(char *shelf){
  int len = strlen(shelf);
  if(!isalpha(shelf[0]) || len < 2){
      return false;
    }
    for(int i = 1; i < len; ++i){
      if(!isdigit(shelf[i])){
          return false;
        }
    }
    return true;
}


bool search_shelves(list_t *shelves, char *shelf){
  int size = list_length(shelves);
  for(int i = 0; i < size; ++i){
    shelf_t *og_shelf = list_get(shelves, i);
    //og_shelf = (shelf_t *)og_shelf;
    int ans = strcmp(og_shelf->shelf, shelf);
    if(ans == 0){
      return true;
    }
  }
  return false;
}

bool shelf_check(tree_t *lager, char *shelf){
  int size = tree_size(lager);
  T *elem_list = tree_elements(lager);
  for(int i = 0; i < size; ++i){
    item_t *item = elem_list[i];
    list_t *shelves = item->shelfs;
    bool x = search_shelves(shelves, shelf);
    if(x){
      return true;
    }
  }
  return false;
}

char *ask_question_shelf(tree_t *lager){
  char *question = "Lägg till hyllnummer";
  char *input = ask_question(question, is_shelf, (convert_func) strdup).s;
  while(1){
    if(!shelf_check(lager, input)){
      break;
    }
    print_message("Hyllan är redan upptagen");
    input = ask_question(question, is_shelf, (convert_func) strdup).s;
  }
  return input;
}

void input_shelf(list_t *item_shelves, tree_t *lager){
  do{
    shelf_t *shelf = calloc(1, sizeof(shelf_t));
    char *shelf_name = ask_question_shelf(lager);
    shelf->shelf = shelf_name;
    shelf->amount = ask_question_int("Lägg in antal");
    list_append(item_shelves, shelf); //append to real list
    print_message("Vill du lägga till en ny hylla?");
  }while(yes_or_no());
}


list_t *input_shelves(tree_t *lager){
  list_t *item_shelves = list_new();
  input_shelf(item_shelves, lager);
  return item_shelves;
}

item_t make_item(char *name, char *desc, int price, list_t *shelfs){
  item_t item = { .name = name, .desc = desc, .price = price, .shelfs = shelfs };
  return item;
}

item_t input_item(tree_t *lager){
  char *name = ask_question_string("Vad är namnet på varan:");
  char *desc = ask_question_string("Beskrivning av vara:");
  int price = ask_question_int("Hur mycket kostar varan:");
  while(price < 1){
    price = ask_question_int("Vänligen lägg in ett riktigt pris:");
  }
  list_t *shelfs = input_shelves(lager);
  item_t item = make_item(name, desc, price, shelfs);
  return item;
}

int add_goods_check(){
  print_message("Ska denna vara läggas till i lagret?");
  puts("[J]a, [N]ej eller [R]edigera");
  while(1){
    char choice = get_a_char();
    if(check_character(choice, 'J')){
      return 1;
    }if(check_character(choice, 'N')){
      return 0;
    }if(check_character(choice, 'R')){
      return -1;
    }
  }
  return 0;
}




int choose_index(int size, char *question){
  int choice = ask_question_int(question);
  while(choice < 1 || choice > size){
      choice = ask_question_int("Var vänlig välj ett nummer i listan");
  }
  return choice;
}

void print_shelf(void *s, void *index){
  shelf_t *shelf = (shelf_t *)s;
  int *i = (int *)index;
  printf("%d. Hylla: %s, Antal: %d\n", *i, shelf->shelf, shelf->amount);
  ++*i;
}

void print_shelves(list_t *shelves){
  int index = 1;
  void *index_pointer = &index;
  puts("-----------------------------");
  list_apply(shelves, print_shelf, index_pointer);
}

void print_item(item_t item){
  puts("---------------------------");
  printf("Namn: %s\n", item.name);
  printf("Beskrivning: %s\n", item.desc);
  printf("Pris: %d\n", item.price);
  print_shelves(item.shelfs);
  puts("---------------------------");


}

void print_node(char *key, void *item, void *index){
  int *i = (int *)index;
  printf("%d. %s\n", *i, key);
  *i = *i + 1;
}

void find_and_print_item(tree_t *lager, int choice){
  K *key_list = tree_keys(lager);
  char *key = key_list[choice-1];
  printf("%s\n", key);
  void *get = tree_get(lager, key);
  item_t *item = (item_t *)get;
  print_item(*item);
}

int print_lager_aux(tree_t *lager, char *question){
  K *key_list = tree_keys(lager);
  int index = 0;
  int offset = 0;
  int size = tree_size(lager);
  while(size != 0){
    printf("%d. %s\n", index+1, key_list[index+offset]);
    ++index;
    --size;
    if(size == 0){ // När vi kommit till sista item
      print_message(question);
      if(yes_or_no()){ // Fråga ifall de vill välja vara
        int choice = choose_index(index, "Välj en vara");
        return choice+offset;
      }else{
	return 0;
      }
    }
    if(index > 19){ // När vi passerat 20 printade items
      puts("Vill du se nästa sida?");
      bool ans = yes_or_no();
      if(!ans){             // Ifall de inte vill gå till nästa sida
        print_message(question);
	if(yes_or_no()){ // fråga ifall de vill välja en vara
	  int choice = choose_index(20, "Välj en vara");
          return choice+offset;  // om ja, retunera valet
        }else{
	  return 0;
	}
      }
      index = 0;
      offset = offset + 20;
    }
  }
  return 0;
}

void print_lager(tree_t *lager){
  int lager_choice = print_lager_aux(lager, "Vill du se mer information om någon vara?");
  if(lager_choice){
    find_and_print_item(lager, lager_choice);
  }
}


int ask_question_edit(){
  char *question = "Vilken del av varan vill du ändra på?\n1.Beskrivningen\n2.Priset\n3.Hyllorna\n";
  int choice = choose_index(3, question);
  return choice;
}

void edit_shelves(list_t *shelf_list, tree_t *lager){
  print_shelves(shelf_list);
  puts("===========================");
  char *question = "1. Vill du lägga till hyllor?\n2. Vill du ta bort en hylla?\n3. Vill du börja om med helt nya hyllor?";
  int choice = choose_index(3, question);

  void *storage = calloc(1, sizeof(void *));
  if (choice == 1){
    input_shelf(shelf_list, lager);
  }else if(choice == 2){
    print_shelves(shelf_list);
    int choice_del = choose_index(list_length(shelf_list), "Vilken hylla vill du ta bort?");
    if(list_remove(shelf_list, choice_del-1, storage)){
      puts("Hyllan är borttagen!");
      free(storage);
    }else{
      puts("Något gick snett, hyllan kunde ej tas bort!");
    }
  }else if(choice == 3){
    while(list_length(shelf_list) != 0){
      list_remove(shelf_list, 0, storage);
    }
    free(storage);
    input_shelf(shelf_list, lager);
  }
}

void edit_item(item_t *item, tree_t *lager){
  print_item(*item);
  int choice = ask_question_edit();
  if(choice == 1){
    printf("Nuvarande beskrivning : %s \n", item->desc);
    puts("----------------------------");
    char *new_desc = ask_question_string("Vad är den nya beskrivningen?");
    strcpy(item->desc, new_desc);
  }else if(choice == 2){
    printf("Nuvarande pris : %d \n", item->price);
    puts("----------------------------");
    int new_price = ask_question_int("Vad är det nya priset?");
    item->price = new_price;
  }else if(choice == 3){
    edit_shelves(item->shelfs, lager);
    puts("Vill du redigera något mer?");
    bool ans = yes_or_no();
    while(ans){
      edit_shelves(item->shelfs, lager);
      puts("Vill du redigera något mer?");
      ans = yes_or_no();
    }
  }
}


void edit_goods(tree_t *lager, action_t *undo){
  int choice = print_lager_aux(lager, "Vill du redigera en av dessa varor");
  K *key_list = tree_keys(lager);
  char *key = key_list[choice-1];
  void *get = tree_get(lager, key);
  item_t *item = (item_t *)get;
  undo->type = 3;
  undo->copy = *item;
  undo->merch = item;
  edit_item(item, lager);
  print_item(*item);
}


void add_goods(tree_t *lager, action_t *undo){
  item_t *item_adress = calloc(1, sizeof(item_t));
  item_t new_item = input_item(lager);
  *item_adress = new_item;
  print_item(*item_adress);

  int choice = add_goods_check();
  while(choice == -1){
    edit_item(item_adress, lager);
    print_item(*item_adress);
      choice = add_goods_check();
  }
  if(choice == 1){
    char *item_name = strdup(item_adress->name);
    printf("%s\n", item_name);
    if(tree_insert(lager, item_name, item_adress)){
      print_message("Varan är tillagd!");
      undo->type = 1;
      undo->merch = item_adress;
    }else{
      print_message("Något gick snett, varan kunde inte läggas till i lagret!");
    }
  }else if(choice == 0){
    print_message("Varan blev inte tillagd!");
  }
}


void undo_add(tree_t *lager, action_t *undo){
  print_message("NOT YET IMPLEMENTED1");
}

void undo_remove(tree_t *lager, action_t *undo){
  print_message("NOT YET IMPLEMENTED2");
}

void undo_edit_aux(item_t *ex_item, item_t og_item){
  ex_item->desc = og_item.desc;
  ex_item->price = og_item.price;
  ex_item->shelfs = og_item.shelfs;
}

void undo_edit(tree_t *lager, action_t *undo){
  undo_edit_aux(undo->merch, undo->copy);
  undo->type = 0;
}


void undo_action(tree_t *lager, action_t *undo){
  if(undo->type == 0){
    print_message("Det finns ingen handling att ångra!");
  }else if(undo->type == 1){
    undo_add(lager, undo);
  }else if(undo->type == 2){
    undo_remove(lager, undo);
  }else if(undo->type == 3){
    undo_edit(lager, undo);
    print_message("Din redigering har ångrats!");
  }
}



void check_choice(char c, tree_t *lager, action_t *undo){
  if(c == 'A'){
    print_message("Programmet avslutas!");
    exit(0);
  }else if(c == 'L'){
    add_goods(lager, undo);    // check på menu valen
  }else if(c == 'H'){
    if(tree_size(lager) > 0){
      print_lager(lager);
    }else{
      print_message("Det finns inget på lagret!");
    }
  }else if(c == 'R'){
    edit_goods(lager, undo);
  }else if(c == 'G'){
    undo_action(lager, undo);
  }
}

action_t *make_undo(){ // initierar undo variabeln
  action_t undo = { .type = 0, .merch = NULL, .copy = NULL };
  action_t *adress = calloc(1, sizeof(action_t));
  *adress = undo;
  return adress;
}

void event_loop(){
  tree_t *lager = tree_new();
  action_t *undo = make_undo();

  while(true){
    print_menu();
    char choice = get_a_char();
    printf("%c\n", choice);
    check_choice(choice, lager, undo);
  }
}

int main(){
  event_loop();
  return 0;
}
