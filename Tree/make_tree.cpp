#include "tree.hpp"
#include <assert.h>



int tree_make_node(Tree **node) {
    assert(node != NULL);

    Tree *new_node = (Tree *) malloc(sizeof(Tree));
    if (new_node == NULL) {
        return TREE_NODE_ALLOC_FAILED;
    }
    DATA(new_node) = {};
    LEFT(new_node) = NULL;
    RIGHT(new_node)= NULL;

    *node = new_node;
    return TREE_OK;
}


int tree_free(Tree *tree) {
    if (tree == NULL) return TREE_OK;

    EXIT_ERR(tree_free(LEFT(tree)));
    data_type_free(DATA(tree));
    EXIT_ERR(tree_free(RIGHT(tree)));

    free(tree);

    return TREE_OK;
}
