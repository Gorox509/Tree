#include <string.h>
#include <assert.h>

static int do_program(int argc, char *argv[]);

#include "tree.hpp"

int main(int argc, char *argv[]) {
    do_program(argc, argv);

    return 0;
}


int do_program(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: <read_file> <write_file>\n");
        return 1;
    }

    Tree *tree = NULL;
    tree_read(argv[1], &tree);

    tree_elem_t val = (tree_elem_t) malloc(sizeof(char) * MAX_STR_LEN);
    while (scanf("%s", val) == 1 && strcmp(val, "!q")) {
        tree_insert(&tree, val);
    }
    free(val);

    tree_print(stdout, tree);
    tree_write(argv[2], tree);

    tree_free(tree);

    return 0;
}


int data_type_free(tree_elem_t data) {
    free(data);
    return TREE_OK;
}

int data_type_cmp(tree_elem_t val1, tree_elem_t val2) {
    return -strcmp(val1, val2);
    //return val2 - val1;
}

tree_elem_t data_type_alloc() {
    return (char *) malloc(sizeof(char) * MAX_STR_LEN);
    //return 0;
}

int data_type_copy(tree_elem_t *dest, const tree_elem_t *src) {
    assert(dest != NULL);
    assert(src != NULL);

    memcpy(*dest, *src, sizeof(char) * MAX_STR_LEN);

    //*dest == *src;
    return 0;
}
