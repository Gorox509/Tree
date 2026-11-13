#include "tree.hpp"
#include <assert.h>
#include <memory.h>

static int tree_insert_left(Tree *tree, tree_elem_t val);
static int tree_insert_right(Tree *tree, tree_elem_t val);


void tree_error_print(int err) {
    switch (err) {
        case TREE_OK:
        default:
            return;

        case TREE_NODE_ALLOC_FAILED:
            fprintf(stderr, "Error: node allocation failed\n");
            return;

        case TREE_CANT_READ:
            fprintf(stderr, "Error: cant read tree from file\n");
            return;

        case TREE_READ_WRONG_SYNTAX:
            fprintf(stderr, "Error: wrong tree syntax\n");
            return;
    }
}


int tree_insert(Tree **tree, tree_elem_t val) {
    assert(tree != NULL);

    if (*tree == NULL) {
        EXIT_ERR(tree_make_node(tree));
        data_type_copy(&DATA(*tree), &val);
        return TREE_OK;
    }

    int cmp = data_type_cmp(DATA(*tree), val);

    if (LEFT(*tree) == NULL && cmp < 0)
        tree_insert_left(*tree, val);
    else if (RIGHT(*tree) == NULL && cmp >= 0)
        tree_insert_right(*tree, val);
    else if (cmp < 0)
        tree_insert(&LEFT(*tree), val);
    else
        tree_insert(&RIGHT(*tree), val);

    return TREE_OK;
}



int tree_insert_left(Tree *tree, tree_elem_t val) {
    assert(tree != NULL);

    tree_node_t *new_node = NULL;
    EXIT_ERR(tree_make_node(&new_node));
    data_type_copy(&DATA(new_node), &val);

    LEFT(tree) = new_node;

    return TREE_OK;
}


int tree_insert_right(Tree *tree, tree_elem_t val) {
    assert(tree != NULL);

    tree_node_t *new_node = NULL;
    EXIT_ERR(tree_make_node(&new_node));
    data_type_copy(&DATA(new_node), &val);

    RIGHT(tree) = new_node;

    return TREE_OK;
}


void tree_print(FILE *fp, Tree *tree) {
    assert(fp != NULL);
    //if (tree == NULL) return;

    fprintf(fp, "(");

    if (tree != NULL) {
        tree_print(fp, LEFT(tree));
        fprintf(fp, "%s", DATA(tree));
        tree_print(fp, RIGHT(tree));
    }

    fprintf(fp, ")");
}


void tree_write(const char *filename, Tree *tree) {
    assert(filename != NULL);

    FILE *fp = safe_fopen(filename, "w");
    if (fp == NULL) return;

    tree_print(fp, tree);
}


FILE *safe_fopen(const char *filename, const char *mode) {
    assert(filename != NULL);
    assert(mode != NULL);

    FILE *fp = fopen(filename, mode);
        if (fp == NULL) {
            ON_TREE_DEBUG(fprintf(stderr, "Error: cannot open the file\n"));
            return NULL;
        }
    return fp;
}
