#include <stdbool.h>
#include "s21_grep.h"


int main(int argc, char *argv[]) {
    _Bool is_error = EXIT_SUCCESS;
    if (grep_function(argc, argv)) {
        is_error = EXIT_FAILURE;
    }
    return is_error;
}

_Bool grep_function(int argc, char *argv[]) {
    _Bool is_error = EXIT_SUCCESS;  // grep return 0 if it is all ok

    grep_flags_struct flags = {0};               // for flags
    int *files_argv = (int *) calloc(argc, sizeof(int));  // for files
    Template *head = NULL;                       // node list for templates

    if (argc < 1) {
        fprintf(stderr, "There is no flags or files!\n");
        is_error = 1;
    } else if (argc == 1) {
        fprintf(stderr, "This grep doesn't work with stdin input!\n");
    } else {
        if (parser(argc, argv, &flags, files_argv, head)) {
            is_error = 1;
        }
    }
    free(files_argv);
    return is_error;
}


bool parser(int argc, char *argv[], grep_flags_struct *flags, int *files_argv, Template *head) {
    _Bool is_error = EXIT_SUCCESS;
    int start_possible_temlate = 1;
    _Bool is_possible_temlate_find = 0;

    for (int i = 1; i < argc && !is_error; i++) {   //TO DO узнать, сбрасывается ли при нахождении ложного флага
        if (argv[i][0] == '-') {
            for (size_t j = 1; j < strlen(argv[i]) && !is_error; j++) {
                if (argv[i][j] == 'e' || argv[i][j] == 'f') {
                    if (i == argc - 1) {
                        fprintf(stderr, "There is no temlates(files with templates) after -e or -f!\n");
                        is_error = 1;
                    } else {
                        if (argv[i][j] == 'e') {
                            flags->e_flag = 1;
                            add_to_end_list(head, argv[i + 1]);
                        } else {
                            flags->f_flag = 1;
                            add_templates_from_file(head, argv[i + 1]);    // TO DO
                        }
                        i++;
                    }
                } else if (argv[i][j] == 'v') { // TO DO
                    flags->v_flag = 1;
                } else if (argv[i][j] == 'c') {
                    flags->c_flag = 1;
                } else if (argv[i][j] == 'l') {
                    flags->l_flag = 1;
                } else if (argv[i][j] == 'n') {
                    flags->n_flag = 1;
                } else if (argv[i][j] == 'i') {
                    flags->i_flag = 1;
                } else if (argv[i][j] == 'o') {
                    flags->o_flag = 1;
                } else if (argv[i][j] == 'h') {
                    flags->h_flag = 1;
                }
            }
        }
    }
    return 0;
}

Template* get_last_element(Template *head) {      //get last element of list
    if (head == NULL) {      // pre-check if list is null
        return NULL;
    }
    while (head->next) {
        head = head->next;
    }
    return head;
}

void add_to_end_list(Template *head, char *value) {
    Template *last = get_last_element(head);
    Template *temp = (Template*) malloc((sizeof(Template)));
    temp->template_text = malloc(strlen(value) + 1);   //TO DO проверить про нуль терминатор через дебагер
    strcpy(temp->template_text, value);     // https://stackoverflow.com/questions/70297807/copy-a-string-to-a-struct-member
    temp->next = NULL;            // https://learnc.info/adt/linked_list.html
    last->next = temp;
}

