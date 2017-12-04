#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct link link_t;

struct list{
  link_t *first;
  link_t *last;
};

struct link{
  L elem;
  link_t *next;
};


//Creates a new empty list and returns it
list_t *list_new(){
  return calloc(1, sizeof(list_t));
}


//creates a new link with the elem input
link_t *link_new(L elem, link_t *next){
  link_t *new_link = calloc(1, sizeof(link_t));
  new_link->elem = elem;
  new_link->next = next;
  return new_link;
}


void list_append (list_t *list, L elem){
  link_t *link = link_new(elem, NULL);
  if(list->last != NULL){
    list->last->next = link;
    list->last = link;
  }else{ //makes the appended item the first and the last adress in list if it's the only one
    list->first = link;
    list->last = link;
  }
}

//prepends elem to the list, which creates a new list entry at the end of that list that points to itself.
void list_prepend (list_t *list, L elem){
  link_t *link = link_new(elem, NULL);
  link_t *tmp = list->first;
  list->first = link;
  link->next = tmp;
  if(list->last == NULL){ //makes the prepended item the first and the last adress in list if it's the only one
    list->last = link;
  }
}


//returns the length of the list
int list_length(list_t *list){
  link_t *cursor = list->first;
  int i = 0;
  while(cursor != NULL){
    cursor = cursor->next;
    ++i;
  }
  return i;
}


bool list_insert_aux(list_t *list, int index, L elem){
  link_t *cursor = list->first;
  
  int i = 0;
  while (i < (index - 1)){ //manouvers to the link which the new link shall be appended to
    cursor = cursor->next;
    ++i;
    printf("I am looping, value of i = %d\n", i);
    }
  link_t *new_next = cursor->next;
  link_t *new_link = link_new(elem, new_next);
  cursor->next = new_link;
  return true;
}


//returns true if the new element was added, false if it wasn't
bool list_insert(list_t *list, int index, L elem){
  int check = abs(index);
  if(list_length(list) <= index || list_length(list) < check){
    return false;
  }else if(index == 0 || (list_length(list) + index == 0)){
    list_prepend(list, elem);
    return true;
  }else if(list_length(list) == index || index == -1){
    list_append(list, elem);
    return true;
  }else if (index < 0){
    index = list_length(list) + index;
    return list_insert_aux(list, index, elem);
  }else{
    return list_insert_aux(list, index, elem);
  }
}



bool list_remove_aux(list_t *list, int index, L *elem){
  if(list->first == list->last){
    free(list->first);
    list->first = NULL;
    list->last = NULL;
    return true;
  }else{
    link_t *cursor = list->first;
    if(index == 0){
      link_t *temp = cursor->next;
      free(cursor);
      list->first = temp;
      return true;
    }
    
    int i = 0;
    while (i < (index - 1)){// sets cursor to the node before the selected node 
      cursor = cursor->next;
      ++i;
    }
    link_t *tmp = cursor->next;
    if(tmp->next == NULL){// if the removed node is the last
      list->last = cursor;
    }
    elem = tmp->elem;
    cursor->next = tmp->next;
    free(tmp);
    return true;
  }
}

bool list_remove(list_t *list, int index, L *elem){
  int check = abs(index);
  if(list_length(list) <= index || list_length(list) < check){
    return false;
  }else if (index < 0){ //fixes negative index
    index = list_length(list) + index;
    return list_remove_aux(list, index, elem);
  }else{
    return list_remove_aux(list, index, elem);
  }
}


L list_get_aux(list_t *list, int index){
  if (index == 0){
    return list->first->elem;
  }
  link_t *cursor = list->first;
  int i = 0;
  while (i <= (index - 1)){
    cursor = cursor->next;
    ++i;
  }
  return cursor->elem;
}

L *list_get(list_t *list, int index){
  int check = abs(index);
  if(list_length(list) <= index || list_length(list) < check){
    printf("The given index doesn't exist\n");
    return 0;
  }else if (index < 0){
    index = list_length(list) + index;
    return list_get_aux(list, index);
  }else{
    return list_get_aux(list, index);
  }
}

void cleanup_list(L elem){
  free(elem);
}

void list_delete(list_t *list, list_action cleanup){
  L *storage = calloc(1, sizeof(L));
  link_t *cursor;
  while(list_length(list) != 0){
    cursor = list->first;
    if(cleanup != NULL){
      cleanup(cursor->elem);
    }
    list_remove(list, 0, storage);
  }
  free(storage);
  free(list);
}


void list_apply(list_t *list, list_action2 fun, void *data){
  if(list_length(list) > 0){
    link_t *cursor = list->first;
    while(cursor->next != NULL){//doesn't catch last link
      fun(cursor->elem, data);
      cursor = cursor->next;
    }
    fun(cursor->elem, data); // catches last link
  }
}



/*
void main(){
  
  void *storage = calloc(1, sizeof(L));
  list_t *list = list_new();
  char *string = "A20";
  char *string2 = "B20";
  char *string3 = "B30";
  char *string4 = "C50";
  link_t *new_link = link_new(string, NULL);
  list_append(list, string);
  list_append(list, string2);
  list_append(list, string3);
  
  bool y = list_insert(list, 0, string4);
  //bool z = list_remove(list, 3, storage);
  int x = list_length(list);
  void *get = list_get(list, 0);
  printf("The element you got was: %s\n", ((char *) get));
  printf("The length of the list is: %d\n", x);
  printf("The last element is: %s\n", ((char *)list->last->elem));
}
*/
