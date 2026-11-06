#include "tree.hpp"
#include <assert.h>

static int tree_insert_left(Tree *tree, tree_elem_t val);
static int tree_insert_right(Tree *tree, tree_elem_t val);



int tree_insert(Tree **tree, tree_elem_t val) {
    assert(tree != NULL);
    if (*tree == NULL) {
        tree_make_node(tree);
        DATA(*tree) = val;
        return TREE_OK;
    }

    int cmp = data_type_cmp(DATA(*tree), val);

    if (LEFT(*tree) == NULL && cmp < 0)
        tree_insert_left(*tree, val);
    else if (RIGHT(*tree) == NULL && cmp > 0)
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
    DATA(new_node) = val;

    LEFT(tree) = new_node;

    return TREE_OK;
}


int tree_insert_right(Tree *tree, tree_elem_t val) {
    assert(tree != NULL);

    tree_node_t *new_node = NULL;
    EXIT_ERR(tree_make_node(&new_node));
    DATA(new_node) = val;

    RIGHT(tree) = new_node;

    return TREE_OK;
}


void tree_print(FILE *fp, Tree *tree) {
    if (tree == NULL) return;

    fprintf(fp, "(");
    tree_print(fp, LEFT(tree));
    fprintf(fp, "%d", DATA(tree));
    tree_print(fp, RIGHT(tree));
    fprintf(fp, ")");
}
