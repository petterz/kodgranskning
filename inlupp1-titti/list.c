#include <stdlib.h>
#include <stdio.h>
#include "list.h"

typedef struct node N;

struct list{
  N* first;
  N* last;
};

struct node{
  N* next;
  L* content;
};

list_t* list_new(){
  return calloc (1, sizeof(list_t));
}

void list_append(list_t* list, L elem){
  N* new = calloc (1, sizeof (N));
  L* new_content = malloc(sizeof(L));
  *new_content = elem;
  new->content = new_content;
  if (list->last != NULL)
    list->last->next = new;
  if (list->first == NULL)
    list->first = new;
  list->last = new;
}

void list_prepend(list_t* list, L elem){
  N* new = calloc(1, sizeof (N));
  L* new_content = malloc(sizeof(L));
  *new_content = elem;
  new->content = new_content;
  new->next = list->first;
  list->first = new;
}

bool list_insert(list_t* list, int index, L elem){
  if (list_length(list) == 0){
    list_append(list, elem);
    return true;
  }
  if (index < 0){
    index += list_length(list) + 1;
  }
  if (index == 0){
    list_prepend(list, elem);
    return true;
  }
  N* current = list->first;
  if (!current)
    return false;
  while (index > 1){
    if (current->next == NULL)
      return false;
    current = current->next;
    index--;
  }
  N* new = calloc(1, sizeof(N));
  L* new_content = malloc(sizeof(L));
  *new_content = elem;
  new->content = new_content;
  new->next = current->next;
  if (new->next == NULL)
    list->last = new;
  current->next = new;
  return true;
}

bool list_remove(list_t* list, int index, L* elem){
  N* current = list->first;
  if (!current)
    return false;
  N* temp;
  if (index < 0)
    index += list_length(list);
  if (index == 0){
    temp = current;
    list->first = temp->next;
  }
  else {
    while (index-- > 1){
      if (current->next == NULL)
        return false;
      current = current->next;
    }
    temp = current->next;
    if (!temp)
      return false;
    current->next = current->next->next;
  }
  if (list->last == temp){
    if (list->first == temp){
      list->first = NULL;
      list->last = NULL;
    }
    else
      list->last = current;
  }
  *elem = *temp->content;
  free(temp);
  return true;
}

L* list_get(list_t *list, int index){
  N* current = list->first;
  if (index < 0){
    int len = list_length(list);
    index = index % len;
    if (index < 0)
      index += len + 1;
  }
  if (current == NULL){
    return NULL;
  }
  while (index-- > 0){
    if (current->next == NULL)
      return NULL;
    current = current->next;
  }
  return current->content;
}

L* list_first(list_t *list){
  if (list->first == NULL)
    return NULL;
  return list->first->content;
}

L* list_last(list_t *list){
  if (list->last == NULL)
    return NULL;
  return list->last->content;
}

int list_length(list_t *list){
  if (list->first == NULL)
    return 0;
  int i = 1;
  N* current = list->first;
  while (current != list->last){
    current = current->next;
    i++;
  }
  return i;
}
