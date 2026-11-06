static int do_program();

#include "tree.hpp"

int main() {
    do_program();

    return 0;
}


int do_program() {
    Tree *tree = NULL;
    int val = 0;

    while (scanf("%d", &val) == 1) {
        tree_insert(&tree, val);
    }

    tree_print(stdout, tree);

    tree_free(tree);

    return 0;
}


int data_type_free(tree_elem_t data) {

    return TREE_OK;
}

int data_type_cmp(tree_elem_t val1, tree_elem_t val2) {
    return val2 - val1;
}
