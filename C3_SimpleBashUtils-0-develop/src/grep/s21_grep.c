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

    grep_flags_struct flags = {0};                           // for flags
    int *files_argv = (int *)calloc(argc + 1, sizeof(int));  // for files
    if (files_argv == NULL) {
        is_error = EXIT_FAILURE;
    }
    Template *template_head = NULL;  // node list for templates
    size_t files_argv_size = 0;

    if (argc < 1) {
        fprintf(stderr, "There is no flags or files!\n");
        is_error = EXIT_FAILURE;
    } else if (argc == 1) {
        fprintf(stderr, "This grep doesn't work with stdin input!\n");
    } else if (!is_error) {
        if (parser(argc, argv, &flags, files_argv, &template_head,
                   &files_argv_size)) {
            is_error = EXIT_FAILURE;
        }
        files_processing(template_head, flags, files_argv, argv, files_argv_size);
    }
    free_template_node(template_head);
    free(files_argv);
    return is_error;
}

_Bool parser(int argc, char *argv[], grep_flags_struct *flags, int *files_argv,
             Template **template_head, size_t *files_count) {
    _Bool is_error = EXIT_SUCCESS;
    for (int i = 1; i < argc && !is_error; i++) {
        if (argv[i][0] == '-') {
            _Bool is_jump = 0, is_e_f_in_flag = 0;
            for (size_t j = 1; j < strlen(argv[i]) && !is_error && !is_e_f_in_flag;
                 j++) {
                if (argv[i][j] == 'e' || argv[i][j] == 'f') {
                    if (i == argc - 1 && j == strlen(argv[i]) - 1) {
                        fprintf(stderr, "There is no templates after -e or -f!\n");
                        is_error = EXIT_FAILURE;
                    } else {
                        char *template;
                        if (j == strlen(argv[i]) - 1)
                            template = argv[i + 1];
                        else
                            template = &argv[i][j + 1];
                        if (argv[i][j] == 'e') {
                            flags->e_flag = 1;
                            if (*template_head == NULL)
                                *template_head = create_node(template);
                            else
                                add_template_to_end_list(*template_head, template);
                        } else {
                            if (add_templates_from_file(template_head, template)) {
                                is_error = EXIT_FAILURE;
                            }
                            flags->f_flag = 1;
                        }
                        if (!is_error) {
                            if (j != strlen(argv[i]) - 1) {
                                is_e_f_in_flag = 1;
                                continue;
                            }
                            is_jump = 1;  //прыжок через одно значение argv при -e или -f
                        }
                    }
                } else if (argv[i][j] == 'v')
                    flags->v_flag = 1;
                else if (argv[i][j] == 'c')
                    flags->c_flag = 1;
                else if (argv[i][j] == 'l')
                    flags->l_flag = 1;
                else if (argv[i][j] == 'n')
                    flags->n_flag = 1;
                else if (argv[i][j] == 'i')
                    flags->i_flag = 1;
                else if (argv[i][j] == 'h')
                    flags->h_flag = 1;
                else if (argv[i][j] == 's')
                    flags->s_flag = 1;
                else {
                    fprintf(stderr, "Illegal Options!\n");
                    is_error = 1;
                }
            }
            if (is_jump) i++;
        } else
            files_argv[(*files_count)++] = i;
    }

    if ((!flags->e_flag && !flags->f_flag) && !is_error) {
        if (!files_argv[0] || !files_argv[1]) {
            fprintf(stderr, "There is no template or file\n");
            is_error = 1;
        } else {
            if (*template_head == NULL)
                *template_head = create_node(argv[files_argv[0]]);
            else
                add_template_to_end_list(*template_head, argv[files_argv[0]]);
            shift_files_array(files_argv);
            (*files_count)--;
        }
    }
    return is_error;
}

Template *create_node(char *value) {
    Template *template_head = (Template *)malloc(sizeof(Template));
    if (template_head == NULL) {
        exit(EXIT_FAILURE);  // подумать, что делать с экзит в методе
    }
    strcpy(template_head->template_text, value);
    template_head->next = NULL;
    return template_head;
}

void add_template_to_end_list(Template *head, char *value) {
    _Bool is_added = 0;
    while (head && !is_added) {
        if (!head->next) {
            head->next = create_node(value);
            is_added = 1;
        } else {
            head = head->next;
        }
    }
}

void free_template_node(Template *head) {
    while (head) {
        Template *temp = head;
        head = head->next;
        free(temp);
    }
}

_Bool add_templates_from_file(Template **head, char *value) {
    _Bool is_error = EXIT_SUCCESS;
    FILE *fp = fopen(value, "r");
    if (fp == NULL) {
        fprintf(stderr, "%s: No such file or directory\n", value);
        is_error = EXIT_FAILURE;
    } else {
        char buffer[BUFFER_IN_FILE];
        while (fgets(buffer, BUFFER_IN_FILE, fp)) {
            size_t len = strlen(buffer);
            if (buffer[len - 1] == '\n' &&
                len != 1) {  // убираем символ переноса на новую строку
                buffer[len - 1] = '\0';
            }
            if (*head == NULL) {
                *head = create_node(buffer);
            } else {
                add_template_to_end_list(*head, buffer);
            }
        }
        if (fclose(fp)) {
            fprintf(stderr, "Can't close file!\n");
        }
    }
    return is_error;
}

void shift_files_array(int *files_argv) {
    int i;
    for (i = 1; files_argv[i]; i++) {
        files_argv[i - 1] = files_argv[i];
    }
    files_argv[i - 1] = 0;  // there is need to cansel last i increment
}

void files_processing(Template *template_node, grep_flags_struct flags,
                      int *files_argv, char *argv[], size_t files_argv_size) {
    int *ptr = files_argv;
    while (*ptr) {
        FILE *fp = fopen(argv[*ptr], "r");
        if (!fp) {
            if (!flags.s_flag) {
                fprintf(stderr, "%s: No such file or directory\n", argv[*ptr]);
            }
        } else {
            common_processing(fp, flags, template_node, argv[*ptr], files_argv_size);
            if (fclose(fp)) {
                fprintf(stderr, "Can't close file!\n");
            }
            // c и l вместе должен быть обработан после всего процесса
        }
        ptr++;
    }
}

void common_processing(FILE *fp, grep_flags_struct flags,
                       Template *template_node, char *filename,
                       size_t files_argv_size) {
    char buffer[BUFFER_IN_FILE];
    _Bool is_find_in_file_for_l = 0, is_not_find = 0;
    int c_count = 0, num_line_in_file = 0;
    while (fgets(buffer, BUFFER_IN_FILE, fp)) {
        num_line_in_file++;  // for n
        int total_find_in_patterns =
                compare_word_patterns(buffer, flags, template_node);
        size_t len = strlen(buffer);
        if (buffer[len - 1] != '\n') {
            strcpy(&buffer[len], "\n");
        }
        if (flags.l_flag && !is_not_find) {  // for l
            is_not_find = 1;
        }
        if (total_find_in_patterns) {
            if (flags.c_flag && !flags.v_flag) {
                c_count++;  // for c
            }
            if (flags.l_flag && !is_find_in_file_for_l) {  // for l
                is_find_in_file_for_l = 1;
            }
            if (!flags.c_flag && !flags.l_flag && !flags.v_flag) {
                if (files_argv_size > 1 && !flags.h_flag) {
                    printf("%s:", filename);
                }
                if (flags.n_flag) {
                    printf("%d:", num_line_in_file);
                }
                printf("%s", buffer);
            }
        } else {
            if (flags.c_flag && flags.v_flag) {
                c_count++;
            }
            if (!flags.c_flag && !flags.l_flag && flags.v_flag) {  // TO DO
                if (files_argv_size > 1 && !flags.h_flag) {
                    printf("%s:", filename);
                }
                if (flags.n_flag) {
                    printf("%d:", num_line_in_file);
                }
                printf("%s", buffer);
            }
        }
    }
    if (flags.c_flag) {
        if (files_argv_size > 1 && !flags.h_flag) {
            printf("%s:", filename);
        }
        if (flags.l_flag) {
            if (flags.v_flag) {
                printf("%d\n", is_not_find);
            } else {
                printf("%d\n", is_find_in_file_for_l);
            }

        } else {
            printf("%d\n", c_count);
        }
    }

    if (flags.l_flag && ((!flags.v_flag && is_find_in_file_for_l) ||
                         (is_not_find && flags.v_flag))) {
        printf("%s\n", filename);
    }
}

int compare_word_patterns(char *buffer, grep_flags_struct flags,
                          Template *template_node) {
    int total_find = 0;
    regex_t regex;
    while (template_node) {
        int regc_res, current_patt;
        if (flags.i_flag) {
            regc_res = regcomp(&regex, template_node->template_text,
                               REG_ICASE | REG_EXTENDED);
        } else {
            regc_res = regcomp(&regex, template_node->template_text, REG_EXTENDED);
        }
        if (regc_res != 0) {
            template_node = template_node->next;
            regfree(&regex);
            continue;
        }
        current_patt = regexec(&regex, buffer, 0, NULL, 0);
        if (!current_patt) total_find++;
        regfree(&regex);
        template_node = template_node->next;
    }
    return total_find;
}
