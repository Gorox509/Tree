#include "tree.hpp"
#include <assert.h>
#include <cstdio>
#include <sys/dir.h>


#define GRAPHVIZ_DIR "graphviz/"
#define GRAPHVIZ_FILE "graphviz.gtree"
#define GRAPHVIZ_PLOT_FILE "img.png"


static FILE *open_graphviz_file(const char *filename);
static int make_graphviz_file(const char *filename);
static int graphviz_print_params(FILE *fp);
static int graphviz_print(FILE *fp, Tree *tree, int level);
static int graphviz_print_node(FILE *fp, Tree *node, int level);



int tree_to_graphviz(Tree *tree) {

    make_graphviz_file(GRAPHVIZ_DIR GRAPHVIZ_FILE);

    FILE *fp = open_graphviz_file(GRAPHVIZ_DIR GRAPHVIZ_FILE);
    if (fp == NULL) return TREE_GRAPHVIZ_FAILED_TO_OPEN_FILE;

    fprintf(fp, "digraph G {\n");
    graphviz_print_params(fp);

    graphviz_print(fp, tree, 0);

    fprintf(fp, "}\n");
    fclose(fp);

    system("dot " GRAPHVIZ_DIR GRAPHVIZ_FILE " -Tpng -o " GRAPHVIZ_DIR GRAPHVIZ_PLOT_FILE);

    return TREE_GRAPHVIZ_OK;
}


FILE *open_graphviz_file(const char *filename) {
    assert(filename != NULL);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open log file\n");
        return NULL;
    }

    return fp;
}


int make_graphviz_file(const char *filename) {
    assert(filename != NULL);
    int err = TREE_GRAPHVIZ_OK;

    if (system("mkdir " GRAPHVIZ_DIR " 2>/dev/null")) {
        ON_TREE_DEBUG(fprintf(stderr, "Warning: directory already exists or cannot be created\n"));
        err = TREE_GRAPHVIZ_FAILED_TO_MAKE_DIR;
    }

    return err;
}


int graphviz_print_params(FILE *fp) {
    assert(fp != NULL);

    fprintf(fp, "rankdir=TB;\n");
    fprintf(fp, "node[fontsize=12];\n");
    fprintf(fp, "splines=line;\n");
    fprintf(fp, "bgcolor=white;\n");

    return TREE_GRAPHVIZ_OK;
}


int graphviz_print(FILE *fp, Tree *tree, int level) {
    assert(fp != NULL);

    if (tree != NULL) graphviz_print_node(fp, tree, level);

    if (tree != NULL) {
        if (LEFT(tree) != NULL) {
            graphviz_print(fp, LEFT(tree),  level + 1);
            fprintf(fp, "node%p -> node%p [ label=\"no\", color=\"green\", dir=none ];\n",
                tree, LEFT(tree));
        }

        if (RIGHT(tree) != NULL) {
            graphviz_print(fp, RIGHT(tree), level + 1);
            fprintf(fp, "node%p -> node%p [ label=\"yes\", color=\"green\", dir=none ];\n",
                tree, RIGHT(tree));
        }
    }

    return TREE_GRAPHVIZ_OK;
}


int graphviz_print_node(FILE *fp, Tree *node, int level) {
    assert(fp != NULL);
    assert(node != NULL);

    const char *color = "black";
    const char *fillcolor = "gray67";

    if (LEFT(node) == NULL && RIGHT(node) == NULL) fillcolor = "olivedrab1";

    //TODO: rank
    fprintf(fp, "node%p[ color = %s , style = filled, fillcolor = %s, shape = Mrecord , "
        "label = \"{" ON_TREE_DEBUG("<address> address = %p | data = ") "%s" ON_TREE_DEBUG(" | { <left> left = %p | <right> right = %p }") " }\" ];\n",
        node, color, fillcolor, ON_TREE_DEBUG(node,) DATA(node) ON_TREE_DEBUG(, LEFT(node), RIGHT(node)));

    return TREE_GRAPHVIZ_OK;
}
