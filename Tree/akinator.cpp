#include "tree.hpp"
#include <assert.h>
#include <string.h>

#define AKINATOR_QUESTION "This object is unknown. What is it?"
#define AKINATOR_REMEMBER "I will remember that"
#define AKINATOR_FIRST_NODE "What object do you like?"

#define BASE_TREE_FILE "database.akinator"

static int akinator_questions(Tree **tree);
static int akinator_add_node (Tree **tree);
static int akinator_add_first_node(Tree **tree);

static void akinator_say(const char *line);
static void akinator_ask(const char *line);
static void akinator_correct(const char *line);
static void akinator_ask_difference(const char *line1, const char *line2);

static bool akinator_answer_is_yes  (char *answer);
static bool akinator_answer_is_no   (char *answer);
static bool akinator_answer_is_quit (char *answer);

static char *fgets_no_newline(char *__restrict s, int n, FILE *__restrict stream);


int run_akinator(int argc, char *argv[]) {
    Tree *tree = NULL;
    bool base_file = 1;
    if (argc == 2)  base_file = true;

    if (base_file)  tree_read(BASE_TREE_FILE, &tree);
    else            tree_read(argv[1], &tree);

    if (tree == NULL) akinator_add_first_node(&tree);
    else akinator_questions(&tree);

    if (base_file)  tree_write(BASE_TREE_FILE, tree);
    else            tree_write(argv[1], tree);

    tree_to_graphviz(tree);

    tree_free(tree);

    return 0;
}


int akinator_questions(Tree **tree) {
    assert(tree != NULL);
    assert(*tree != NULL);

    akinator_ask(DATA(*tree));

    char answer[MAX_STR_LEN] = "";
    fgets_no_newline(answer, MAX_STR_LEN, stdin);

    if (akinator_answer_is_quit(answer)) {
        return TREE_OK;
    }

    if (akinator_answer_is_yes(answer)) {
        if (LEFT(*tree) == NULL && RIGHT(*tree) == NULL) {
            akinator_correct(DATA(*tree));
            return TREE_OK;
        }
        return akinator_questions(&RIGHT(*tree));
    }

    if (akinator_answer_is_no(answer)) {
        if (LEFT(*tree) == NULL && RIGHT(*tree) == NULL) return akinator_add_node(tree);
        return akinator_questions(&LEFT(*tree));
    }
    else return akinator_questions(tree);

    return TREE_OK;
}


int akinator_add_node(Tree **tree) {
    assert(tree != NULL);
    assert(*tree != NULL);

    akinator_say(AKINATOR_QUESTION);

    char new_node_data[MAX_STR_LEN] = "";
    fgets_no_newline(new_node_data, MAX_STR_LEN, stdin);

    EXIT_ERR(tree_make_node(&LEFT(*tree)));
    EXIT_ERR(tree_make_node(&RIGHT(*tree)));
    strncpy(DATA(LEFT(*tree)), DATA(*tree), MAX_STR_LEN);   //previous node data moves to left ("no" side)
    strncpy(DATA(RIGHT(*tree)), new_node_data, MAX_STR_LEN); //new node goes to right ("yes" side)

    akinator_ask_difference(new_node_data, DATA(*tree));
    char difference[MAX_STR_LEN] = "";
    fgets_no_newline(difference, MAX_STR_LEN, stdin);

    strncpy(DATA(*tree), difference, MAX_STR_LEN);

    akinator_say(AKINATOR_REMEMBER);

    return TREE_OK;
}


int akinator_add_first_node(Tree **tree) {
    assert(tree != NULL);

    akinator_say(AKINATOR_FIRST_NODE);

    EXIT_ERR(tree_make_node(tree));
    fgets_no_newline(DATA(*tree), MAX_STR_LEN, stdin);

    akinator_say(AKINATOR_REMEMBER);

    return TREE_OK;
}


void akinator_say(const char *line) {
    assert(line != NULL);

    printf("%s\n", line);

    char espeak_cmd[MAX_STR_LEN] = "";
    strcat(espeak_cmd, "espeak \"");
    strcat(espeak_cmd, line);
    strcat(espeak_cmd, "\" 2> /dev/null");
    system(espeak_cmd);
}


void akinator_ask(const char *line) {
    assert(line != NULL);

    char line_to_say[MAX_STR_LEN] = "";
    strcat(line_to_say, "Is it ");
    strcat(line_to_say, line);
    strcat(line_to_say, "?");
    akinator_say(line_to_say);
}


void akinator_correct(const char *line) {
    assert(line != NULL);

    char line_to_say[MAX_STR_LEN] = "";
    strcat(line_to_say, "Of course it was ");
    strcat(line_to_say, line);
    strcat(line_to_say, "!");
    akinator_say(line_to_say);
}


void akinator_ask_difference(const char *line1, const char *line2) {
    assert(line1 != NULL);
    assert(line2 != NULL);

    char line_to_say[MAX_STR_LEN] = "";
    strcat(line_to_say, "OK. How is ");
    strcat(line_to_say, line1);
    strcat(line_to_say, " different from ");
    strcat(line_to_say, line2);
    strcat(line_to_say, "?");

    akinator_say(line_to_say);
}


bool akinator_answer_is_yes(char *answer) {
    assert(answer != NULL);

    if (!strcmp(answer, "Y") || !strcmp(answer, "y") || !strcmp(answer, "yes") || !strcmp(answer, "1")) return true;

    return false;
}


bool akinator_answer_is_no(char *answer) {
    assert(answer != NULL);

    if (!strcmp(answer, "N") || !strcmp(answer, "n") || !strcmp(answer, "no") || !strcmp(answer, "0")) return true;

    return false;
}


bool akinator_answer_is_quit(char *answer) {
    assert(answer != NULL);

    if (!strcmp(answer, "!q")) return true;

    return false;
}


char *fgets_no_newline(char *__restrict s, int n, FILE *__restrict stream) {
    char *return_value = fgets(s, n, stream);

    int idx = 0;
    while (s[idx] != '\n' && s[idx] != '\0') ++idx;

    if (s[idx] == '\n') s[idx] = '\0';

    return return_value;
}
