#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <stdio.h>

typedef int tree_elem_t;

typedef struct tree_node_t {
    tree_elem_t data;
    tree_node_t *left;
    tree_node_t *right;
} Tree;

#define DATA(tree)  (tree)->data
#define LEFT(tree)  (tree)->left
#define RIGHT(tree) (tree)->right


#define TREE_OK                 0
#define TREE_NODE_ALLOC_FAILED  1


#define EXIT_ERR(...)  { int err = TREE_OK; if ((err = __VA_ARGS__) != TREE_OK) exit(err); }
#define CHECK_ERR(...) { int err = TREE_OK; if ((err = __VA_ARGS__) != TREE_OK) return err; }



int data_type_free(tree_elem_t);
int data_type_cmp(tree_elem_t, tree_elem_t);


int tree_make_node(Tree **node);
int tree_free(Tree *tree);
int tree_insert(Tree **tree, tree_elem_t val);
void tree_print(FILE *fp, Tree *tree);

#endif
