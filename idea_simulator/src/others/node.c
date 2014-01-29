/**
 * \file node.c
 * \brief Defines the node structure linking the caches
 * \author ~gmarait
 * \version 1.0
 * \date 29th january 2014
 * 
 * 
 *
 */

#include <string.h>
#include <unistd.h>
#include "node.h"

static unsigned int current_id = 1;

struct node * init_node(){
  struct node * n = malloc(sizeof(struct node));
  if(n == NULL){
    fprintf(stderr, "Node allocation error\n");
    _exit(1);
  }
  n->parent = NULL;
  n->next_sibling = n; //Avoids a next_sibling to be NULL
  n->child = NULL;
  n->nb_children = 0;
  n->id = current_id++;
  n->data = NULL;
}

struct node * get_parent(struct node * n){
  return n->parent;
}

struct node * get_sibling(struct node * n){
  return n->next_sibling;
}

struct node * get_child(struct node * n, unsigned int i){
  if(n->child == NULL || i >= n->nb_children)
    return NULL;
  return n->child[i];
}

struct cache * get_cache(struct node * n){
  return n->data;
}

struct node * get_root(struct node * n){
  struct node * root = n;
  while(get_parent(root)){
    root = get_parent(root);
  }
  return root;
}

int add_child(struct node * parent, struct node * child){
  parent->child = realloc(parent->child, (parent->nb_children + 1) * sizeof(struct node *));
  if(parent->child == NULL){
    fprintf(stderr, "Allocation error\n");
    _exit(1);
  }
  parent->child[parent->nb_children++] = child;
  return 1;
}

void free_node(struct node * n){
  free(n->child);
  free(n);
}
