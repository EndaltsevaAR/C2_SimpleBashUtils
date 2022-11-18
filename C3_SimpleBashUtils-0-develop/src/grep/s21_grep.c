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

_Bool parser(int argc, char *argv[], grep_flags_struct *flags, int *files_argv, Template *head) {
    _Bool is_error = EXIT_SUCCESS;
    int files_count = 0;

    for (int i = 1; i < argc && !is_error; i++) {   //TO DO узнать, сбрасывается ли при нахождении ложного флага
        if (argv[i][0] == '-') {
            for (size_t j = 1; j < strlen(argv[i]) && !is_error; j++) {
                if (argv[i][j] == 'e' || argv[i][j] == 'f') {
                    if (i == argc - 1) {
                        fprintf(stderr, "There is no templates(files with templates) after -e or -f!\n");
                        is_error = 1;
                    } else {        // рассмотреть случай, когда оба в одном
                        if (argv[i][j] == 'e') {
                            flags->e_flag = 1;
                            add_to_end_list(head, argv[i + 1]);
                        } else {
                            flags->f_flag = 1;
                            if (add_templates_from_file(head, argv[i + 1])) {
                                is_error = 1;
                            }    // TO DO
                        }
                        if (!is_error) i++;
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
                } else {
                    fprintf(stderr, "Illegal Options!\n");
                    is_error = 1;
                }
            }
        } else {
            files_argv[files_count++] = i;
        }
    }

    if (!flags->e_flag && !flags->f_flag && !is_error) {    //если нет, ни e флага, ни f флага, то в качестве шаблона используется первый после флагов
        if (!files_argv[0] || !files_argv[1]) {  //первый должен быть шаблон, второй файл
            fprintf(stderr, "There is no template or file\n");
            is_error = 1;
        } else {
        //    add_to_end_list(head, argv[files_argv[0]]);
            shift_files_array(files_argv);
        }
    }
    return is_error;
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
    if (head) {
        last->next = temp;
    } else {
        head = &temp;
    }
}

_Bool add_templates_from_file(Template *head, char *value) {
    _Bool is_error = 0;
    FILE *fp = fopen(value, "r");
    if (fp == NULL) {
        fprintf(stderr, "Can't open file!\n");
        is_error = 1;
    }
    if (!is_error) {
        char buffer[BUFFER] = {0};
        if (fclose(fp)) {
            fprintf(stderr, "Can't close file!\n");
            is_error = 1;
        }
        while (fgets(buffer, BUFFER, fp)) {
            add_to_end_list(head, buffer);
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