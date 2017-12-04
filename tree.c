#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node node_t;

struct node {
  K key;
  T elem;
  node_t *n1;
  node_t *n2;
};

struct tree {
  node_t *node;
};




//Skapar ett nytt, tomt träd. Allokelar minne för storleken

tree_t *tree_new(){
  tree_t *tree = calloc(1, sizeof(tree_t));
  return tree;
}

node_t *node_new(K key, T elem, node_t *n1, node_t *n2){
  node_t *node = calloc(1, sizeof(node_t));
  node->key = key;
  node->elem = elem;
  node->n1 = n1;
  node->n2 = n2;
  return node;
}
  


//Använder sig av rekursion for att gå igenom trädet och retunera det totala antalet noder.



int tree_size_aux(node_t *node){
  if (node->n1 == NULL && node->n2 == NULL){
    return 1;
  }else if(node->n1 == NULL){
    return 1 + tree_size_aux(node->n2);
  }else if(node->n2 == NULL){
    return 1 + tree_size_aux(node->n1);
  }else{
    return 1 + tree_size_aux(node->n2) + tree_size_aux(node->n1);
  }
}

int tree_size(tree_t *tree){
  if(tree->node == NULL){
    return 0;
  }else{
  return tree_size_aux(tree->node);
  }
}


// Använder sig av rekursion för att retunera det största djupet på trädet.

int tree_depth_aux(node_t *node){
  if(node->n1 == NULL && node->n2 == NULL){
    return 1;
  }else if(node->n2 == NULL){
    return 1 + tree_depth_aux(node->n1);
  }else if(node->n1 == NULL){
    return 1 +tree_depth_aux(node->n2);
  }else{
    if(tree_depth_aux(node->n1) >= tree_depth_aux(node->n2)){
      return 1 + tree_depth_aux(node->n1);
    }else{
      return 1 + tree_depth_aux(node->n2);
    }
  }
}

int tree_depth(tree_t *tree){
  if(tree->node == NULL){
    return 0;
  }else{
    return tree_depth_aux(tree->node);
  } 
}


//Använder rekursion för att traversera trädet och retunera jämförelsen av instoppade nyckeln och alla nycklar i trädet, för att se om nyckeln redan finns i trädet.

bool tree_has_key_aux(node_t *node, K key){
  if(!strcmp(node->key, key)){
    return true;
  }else if(node->n1 == NULL && node->n2 == NULL){
    return false;
  }else if(node->n1 == NULL){
    return(tree_has_key_aux(node->n2, key));
  }else if(node->n2 == NULL){
    return(tree_has_key_aux(node->n1, key));
  }else{
    return(tree_has_key_aux(node->n1, key) || tree_has_key_aux(node->n2, key));
  }
}

bool tree_has_key(tree_t *tree, K key){
  if(tree->node == NULL){
    return false;
  }else{
    return tree_has_key_aux(tree->node, key);           
  }
}


//insertar en key och ett element in i ett träd. Om key redan finns i trädet retuneras false


bool tree_insert_aux(node_t *node, K key, T elem){
  if(strcmp(node->key, key) < 0){
    if(node->n2 == NULL){
      node_t *new_node = node_new(key, elem, NULL, NULL);
      node->n2 = new_node;
      return true;
    }else{
      return tree_insert_aux(node->n2, key, elem);
    }
  }else{
    if(node->n1 == NULL){
      node_t *new_node = node_new(key, elem, NULL, NULL);
      node->n1 = new_node;
      return true;
    }else{
      return tree_insert_aux(node->n1, key, elem);
    }
  }
}

bool tree_insert(tree_t *tree, K key, T elem){
  if(tree_size(tree) == 0){
    node_t *new_node = node_new(key, elem, NULL, NULL); 
    tree->node = new_node;
    return true;
  }else if(tree_has_key(tree, key)){ //retunerar false om nyckeln redan finns
      return false;
  }else{
    return tree_insert_aux(tree->node, key, elem);
  }
}


//Utifrån antagandet att den inmatade nyckeln existerar i det givna trädet så kommer funktionen att retunera elementet kopplat till nyckeln


T tree_get_aux(node_t *node, K key){
  if(!strcmp(node->key, key)){
    return node->elem;
  }else if(node->n1 == NULL && node->n2 == NULL){
    return NULL;
  }else if(node->n1 == NULL){
    return tree_get_aux(node->n2, key);
  }else if(node->n2 == NULL){
    return tree_get_aux(node->n1, key);
  }else{
    if(tree_has_key_aux(node->n1, key)){
      return tree_get_aux(node->n1, key);
    }else if(tree_has_key_aux(node->n2, key)){
      return tree_get_aux(node->n2, key); //////////// CHANGE THIS BUGGGGGGGGG
    }else{
      return NULL;
    }
  }
}

T tree_get(tree_t *tree, K key){
  if(tree->node != NULL){
    return tree_get_aux(tree->node, key);
  }
  else{
    return NULL;
  }
}



void cleanup_tree(K key, T elem){
  free(key);
  free(elem);
}

void tree_delete_aux(node_t *node, tree_action cleanup){
  if(node->n1 == NULL && node->n2 == NULL){
    if(cleanup != NULL){
        cleanup(node->key, node->elem);
    }
    free(node);
  }else if(node->n1 == NULL){
    tree_delete_aux(node->n2, cleanup);
  }else if(node->n2 == NULL){
    tree_delete_aux(node->n1, cleanup);
  }else{
    tree_delete_aux(node->n1, cleanup);
    tree_delete_aux(node->n2, cleanup);
  }
}


void tree_delete(tree_t *tree, tree_action cleanup){
  while(tree_size(tree) > 1){
    tree_delete_aux(tree->node, cleanup);
  }
  if (tree_size(tree) == 0){
    free(tree);
  }else{
    free(tree->node);
    free(tree);
    }
}



void tree_keys_aux(node_t *node, K *key_array, int *index){
  if(node->n1 == NULL){//Found the leftmost key of a given sub-tree
    key_array[*index] = node->key;
    *index = *index + 1;
  }else{ // Parent node, recurses into left node, then adds self
    tree_keys_aux(node->n1, key_array, index);
    key_array[*index] = node->key;
    *index = *index + 1;
  }
  
  if(node->n2 != NULL){
    tree_keys_aux(node->n2, key_array, index);
  }
}

K *tree_keys(tree_t *tree){
  if(tree->node == NULL){
    return NULL;
  }else{
    int node_amount = tree_size(tree);
    K *key_array = calloc(node_amount, sizeof(K));
    int index = 0;
    tree_keys_aux(tree->node, key_array, &index);
    return key_array;
  }
}

void tree_elements_aux(node_t *node, T *elem_array, int *index){
    if(node->n1 == NULL){//Found the leftmost key of a given sub-tree
    elem_array[*index] = node->elem;
    *index = *index + 1;
  }else{ // Parent node, recurses into left node, then adds self
    tree_elements_aux(node->n1, elem_array, index);
    elem_array[*index] = node->elem;
    *index = *index + 1;
  }
  
  if(node->n2 != NULL){
    tree_elements_aux(node->n2, elem_array, index);
  }
}

T *tree_elements(tree_t *tree){
  if(tree->node == NULL){
    return NULL;
  }else{
    int node_amount = tree_size(tree);
    T *elem_array = calloc(node_amount, sizeof(T));
    int index = 0;
    tree_elements_aux(tree->node, elem_array, &index);
    return elem_array;
  }
}

 
void tree_apply_aux(node_t *node, enum tree_order order, tree_action2 function, void *data){
  if(order){ // postorder
    if(node->n1 == NULL && node->n2 == NULL){
      function(node->key, node->elem, data);
    }else{
      if(node->n2 == NULL){
        tree_apply_aux(node->n1, order, function, data);
        function(node->key, node->elem, data);
      }else if(node->n1 == NULL){
        tree_apply_aux(node->n2, order, function, data);
        function(node->key, node->elem, data);
      }else{
        tree_apply_aux(node->n1, order, function, data);
        tree_apply_aux(node->n2, order, function, data);
        function(node->key, node->elem, data);
      }
    }
  }
  else if(order == 0){ // inorder
    if(node->n1 == NULL && node->n2 == NULL){
      function(node->key, node->elem, data);
    }else {
      if(node->n1 == NULL){
	function(node->key, node->elem, data);
        tree_apply_aux(node->n2, order, function, data);
      }else if(node->n2 == NULL){
	tree_apply_aux(node->n1, order, function, data);
	function(node->key, node->elem, data);
      }else{
        tree_apply_aux(node->n1, order, function, data);
        function(node->key, node->elem, data);
        tree_apply_aux(node->n2, order, function, data);
      }
    }
  }
  else{ // preorder
    if(node->n1 == NULL && node->n2 == NULL){
      function(node->key, node->elem, data);
    }else {
      if(node->n2 == NULL){
        function(node->key, node->elem, data);
        tree_apply_aux(node->n1, order, function, data);
      }else if(node->n1 == NULL){
        function(node->key, node->elem, data);
        tree_apply_aux(node->n2, order, function, data);
      }else{
        function(node->key, node->elem, data);
        tree_apply_aux(node->n1, order, function, data);
        tree_apply_aux(node->n2, order, function, data);
      }
    }
  }
}


void tree_apply(tree_t *tree, enum tree_order order, tree_action2 fun, void *data){
  if(tree->node == NULL){
    puts("Please input a non-empty tree");
  }else{
    tree_apply_aux(tree->node, order, fun, data);
  }
}







/*
int main(){
  tree_t *tree = tree_new();
  int x = 10;
  int y = 20;
  int z = 30;
  tree_insert(tree, "Aang", &x);
  tree_insert(tree, "Anka", &y);
  tree_insert(tree, "aaaa", &z);
  if(tree_has_key(tree, "aa")){
    puts("Found your key!");
  }
  int depth = tree_depth(tree);
  int size = tree_size(tree);
  printf("Depth of tree: %d\nSize of tree: %d\n", depth, size);
  T *elem_array = tree_elements(tree);
  for (int i = 0; i < tree_size(tree); ++i){
    printf("%d. %d\n", i, *((int *)elem_array[i]));
  }
}
*/  
