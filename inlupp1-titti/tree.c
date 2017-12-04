#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct node
{
  K key;
  struct node *left;
  struct node *right;
  T elem;
} node_t;

struct tree
{
  node_t *root;
};

tree_t *tree_new()
{
  tree_t *alloc = calloc(1, sizeof(tree_t));
  return alloc;
}

void tree_delete_r(node_t* node, tree_action cleanup){
  if (node == NULL)
    return;
  //first delete both subtrees
  tree_delete_r(node->left, cleanup);
  tree_delete_r(node->right, cleanup);

  //use cleanup
  if (cleanup != NULL)
    cleanup(node->key, node->elem);
  
  //then delete the node
  free(node);
}

void tree_delete(tree_t *tree, tree_action cleanup)
{
  tree_delete_r(tree->root, cleanup);
  free(tree);
}

int tree_size_r(node_t* node){
  if (node == NULL)
    return 0;
  return 1 + tree_size_r(node->left) + tree_size_r(node->right);
}

int tree_size(tree_t *tree)
{
  if (tree->root == NULL)
    {
      return 0;
    }
  else
    {
      return tree_size_r(tree->root);
    }
}

int tree_depth_r(node_t* node){
  if(node == NULL)
    return 0;
  int leftDepth = tree_depth_r(node->left);
  int rightDepth = tree_depth_r(node->right);

  if(leftDepth > rightDepth)
    {
      return (leftDepth + 1);
    }
  else
    {
      return (rightDepth + 1);
    }
}

int tree_depth (tree_t *tree)
{
  if(tree->root == NULL)
    {
      return 0;
    }
  return tree_depth_r(tree->root);
}

bool tree_insert(tree_t *tree, K key, T elem)
{
  node_t** node = &tree->root;
  while (true){
    if (*node == NULL){
      *node = calloc(1, sizeof(node_t));
      (*node)->key = key;
      (*node)->elem = elem;
      return true;
    }
    int cmp = strcmp(key, (*node)->key);
    if (cmp == 0)
      return false;
    if (cmp < 0)
      node = &(*node)->left;
    else
      node = &(*node)->right;
  }
}

bool tree_has_key(tree_t *tree, K key)
{
  node_t* node = tree->root;
  while(true){
    if (node == NULL)
      return false;
    int cmp = strcmp(key, node->key);
    if (cmp == 0)
      return true;
    if (cmp < 0)
      node = node->left;
    else
      node = node->right;
  }
}

T tree_get(tree_t *tree, K key)
{
  node_t* node = tree->root;
  while(true){
    if (node == NULL)
      return NULL;
    int cmp = strcmp(key, node->key);
    if (cmp == 0)
      return node->elem;
    if (cmp < 0)
      node = node->left;
    else
      node = node->right;
  }
}

/* next assignment
T tree_remove(tree_t *tree, K key)
{

} */
int get_keys(node_t* node, K* key_list, int list_siz){
  if (node == NULL){
    return list_siz;
  }
  key_list[list_siz] = node->key;
  list_siz++;
  list_siz = get_keys(node->left, key_list, list_siz);
  return get_keys(node->right, key_list, list_siz);
}

K* tree_keys(tree_t* t){
  int siz = tree_size(t);
  K* key_arr = calloc(siz, sizeof(K));
  get_keys(t->root, key_arr, 0);
  K* sort_arr = calloc(siz, sizeof(K));
  for(int i = 0; i < siz; i++){
    char* best = NULL;
    int best_index = 0;
    for (int j = 0; j < siz; j++){
      if (best == NULL)
        best = key_arr[j];
      else {
        if (key_arr[j] != NULL && strcmp(best, key_arr[j]) > 0){
          best = key_arr[j];
          best_index = j;
        }
      }
    }
    sort_arr[i] = best;
    key_arr[best_index] = NULL;
  }
  free(key_arr);
  return sort_arr;
}

int get_elements(node_t* node, T* elem_list, int list_siz){
  if (node == NULL){
    return list_siz;
  }
  elem_list[list_siz] = node->elem;
  list_siz++;
  list_siz = get_elements(node->left, elem_list, list_siz);
  return get_elements(node->right, elem_list, list_siz);
}

T* tree_elements(tree_t* t){
  int siz = tree_size(t);
  T* elem_arr = calloc(siz, sizeof(T));
  get_elements(t->root, elem_arr, 0);
  return elem_arr;
}
