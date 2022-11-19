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
    int *files_argv = (int *)calloc(argc - 1, sizeof(int));  // for files
    Template *template_head = NULL;                       // node list for templates

    if (argc < 1) {
        fprintf(stderr, "There is no flags or files!\n");
        is_error = 1;
    } else if (argc == 1) {
        fprintf(stderr, "This grep doesn't work with stdin input!\n");
    } else {
        if (parser(argc, argv, &flags, files_argv, &template_head)) {
            is_error = 1;
        }
    }
    printf("By!");
    free_template_node(template_head);
    free(files_argv);
    return is_error;
}

_Bool parser(int argc, char *argv[], grep_flags_struct *flags, int *files_argv, Template **template_head) {
    _Bool is_error = EXIT_SUCCESS;
    size_t files_count = 0;
    for (int i = 1; i < argc && !is_error; i++) {   //TO DO узнать, сбрасывается ли при нахождении ложного флага
        _Bool is_jump = 0;
        if (argv[i][0] == '-') {
            for (size_t j = 1; j < strlen(argv[i]) && !is_error; j++) {
                if (argv[i][j] == 'e' || argv[i][j] == 'f') {
                    if (i == argc - 1) {
                        fprintf(stderr, "There is no templates(files with templates) after -e or -f!\n");
                        is_error = 1;
                    } else {        // рассмотреть случай, когда оба в одном
                        if (argv[i][j] == 'e') {
                            flags->e_flag = 1;
                            if (*template_head == NULL) *template_head = create_node(argv[i + 1]);
                            else add_template_to_end_list(*template_head, argv[i + 1]);
                        } else {
                            flags->f_flag = 1;
                            if (add_templates_from_file(template_head, argv[i + 1])) is_error = 1;
                        }
                        if (!is_error) is_jump = 1;  //прыжок через одно значение argv при -e или -f должен быть после того, как парсер пройдет все текущее значение
                    }
                } else if (argv[i][j] == 'v')  flags->v_flag = 1;
                else if (argv[i][j] == 'c') flags->c_flag = 1;
                else if (argv[i][j] == 'l') flags->l_flag = 1;
                else if (argv[i][j] == 'n') flags->n_flag = 1;
                else if (argv[i][j] == 'i') flags->i_flag = 1;
                else if (argv[i][j] == 'o') flags->o_flag = 1;
                else if (argv[i][j] == 'h') flags->h_flag = 1;
                else {
                    fprintf(stderr, "Illegal Options!\n");
                    is_error = 1;
                }
            }
            if (is_jump) i++;
        } else files_argv[files_count++] = i;
    }

    if ((!flags->e_flag || !flags->f_flag) && !is_error) {    //если нет, ни e флага, ни f флага, то в качестве шаблона используется первый после флагов
        if (!files_argv[0] || !files_argv[1]) {  //первый должен быть шаблон, второй файл
            fprintf(stderr, "There is no template or file\n");
            is_error = 1;
        } else {
            if (*template_head == NULL) *template_head = create_node(argv[files_argv[0]]);
            else add_template_to_end_list(*template_head, argv[files_argv[0]]);
            shift_files_array(files_argv);
        }
    }
    return is_error;
}

Template *create_node(char *value) {
    Template *template_head = (Template *)malloc(sizeof(Template));
    if (template_head == NULL) {
        exit(EXIT_FAILURE);            // подумать, что делать с экзит в методе
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
    _Bool is_error = 0;
    FILE *fp = fopen(value, "r");
    if (fp == NULL) {
        fprintf(stderr, "Can't open file!\n");
        is_error = 1;
    }
    if (!is_error) {
        char buffer[BUFFER_IN_FILE];
        while (fgets(buffer, BUFFER_IN_FILE, fp)) {
            size_t len = strlen(buffer);
            if (buffer[len - 1] == '\n' && len != 1) { // убираем символ переноса на новую строку
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
            is_error = 1;
        }
    }
    return is_error;
}

void shift_files_array(int *files_argv) {
    int i;
    for (i = 1; files_argv[i]; i++) {
        files_argv[i-1] = files_argv[i];
    }
    files_argv[i] = 0;
}