#include "tree.hpp"
#include <assert.h>
#include <sys/stat.h>



static int make_buffer_for_reading(char **buf, const char *filename);
static int read_file_to_buffer(char *buf, int buf_len, FILE *fp);
static int tree_parse_node(char *buf, Tree **tree, int *parsed);
static int tree_parse_data(char *buf, Tree **tree, int *parsed);



int tree_read(const char *filename, Tree **tree) {
    assert(filename != NULL);
    assert(tree != NULL);

    FILE *fp = safe_fopen(filename, "r");
    if (fp == NULL) return TREE_CANT_READ;

    char *buf = NULL;
    int buf_len = make_buffer_for_reading(&buf, filename);
    if (buf_len == TREE_CANT_READ) return TREE_CANT_READ;
    read_file_to_buffer(buf, buf_len, fp);

    fclose(fp);

    int parsed = 0;
    EXIT_ERR(tree_parse_node(buf, tree, &parsed))
    free(buf);

    return TREE_OK;
}


int tree_parse_node(char *buf, Tree **tree, int *parsed) {
    assert(buf != NULL);
    assert(tree != NULL);
    assert(parsed != NULL);

    if (buf[(*parsed)++] != '(') return TREE_READ_WRONG_SYNTAX;

    if (buf[*parsed] == ')') {
        ++(*parsed);
        return TREE_OK;
    }
    if (*tree == NULL) {
        EXIT_ERR(tree_make_node(tree));
    }

    ON_READ_DEBUG(fprintf(stderr, "scanning left...\n"));
    EXIT_ERR(tree_parse_node(buf, &LEFT(*tree), parsed));
    ON_READ_DEBUG(fprintf(stderr, "left scanned\n"));

    ON_READ_DEBUG(fprintf(stderr, "scanning data...: "));
    EXIT_ERR(tree_parse_data(buf, tree, parsed));
    ON_READ_DEBUG(fprintf(stderr, "data scanned\n"));

    ON_READ_DEBUG(fprintf(stderr, "scanning right...\n"));
    EXIT_ERR(tree_parse_node(buf, &RIGHT(*tree), parsed));
    ON_READ_DEBUG(fprintf(stderr, "right scanned\n"));

    if (buf[(*parsed)++] != ')') return TREE_READ_WRONG_SYNTAX;

    ON_READ_DEBUG(fprintf(stderr, "node successfully read\n"));

    return TREE_OK;
}


int tree_parse_data(char *buf, Tree **tree, int *parsed) {
    assert(buf != NULL);
    assert(tree != NULL);
    assert(parsed != NULL);

    int scanned = 0;
    sscanf(buf + *parsed, "%[^()]%n", DATA(*tree), &scanned);
    *parsed += scanned;
    ON_READ_DEBUG(fprintf(stderr, "%s\n", DATA(*tree)));

    return TREE_OK;
}


int make_buffer_for_reading(char **buf, const char *filename) {
    assert(buf != NULL);
    assert(filename != NULL);

    struct stat filestat = {};
    stat(filename, &filestat);

    *buf = (char *) calloc((size_t) filestat.st_size + 1, sizeof(char));
    if (*buf == NULL) {
        ON_READ_DEBUG(fprintf(stderr, "Error: buffer allocation failed"));
        return TREE_CANT_READ;
    }
    (*buf)[filestat.st_size] = '\0';
    return (int) filestat.st_size + 1;
}


int read_file_to_buffer(char *buf, int buf_len, FILE *fp) {
    assert(buf != NULL);
    assert(fp != NULL);

    fread(buf, (size_t) buf_len, 1, fp);

    return TREE_OK;
}
