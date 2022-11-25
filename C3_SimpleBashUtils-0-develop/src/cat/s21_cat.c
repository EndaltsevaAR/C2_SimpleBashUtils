#include "s21_cat.h"

int main(int argc, char *argv[]) {
    _Bool is_error = EXIT_SUCCESS;
    if (cat_function(argc, argv)) {
        is_error = EXIT_FAILURE;
    }
    return is_error;
}

_Bool cat_function(int argc, char *argv[]) {
    flags_struct flags = {0};
    _Bool is_error = 0;  // cat return 0 if it is all ok
    if (argc < 1) {
        fprintf(stderr, "There is no flags or files!\n");
        is_error = 1;
    } else if (argc == 1) {
        fprintf(stderr, "This cat doesn't work with stdin input!\n");
    } else {
        int start_files = do_parser(argc, argv, &flags);  // parser
        if (start_files < 0) {
            is_error = 1;
        } else {
            show_files(start_files, argc, argv, flags);
        }
    }
    return is_error;
}

int do_parser(int argc, char **argv, flags_struct *flags) {
    _Bool is_flags_end = 0;
    int start_files = -1;
    for (int i = 1; i < argc && !is_flags_end; i++) {
        if (argv[i][0] == '-') {
            if (!strcmp(argv[i], "--number-nonblank"))
                flags->b_flag = 1;
            else if (!strcmp(argv[i], "--number"))
                flags->n_flag = 1;
            else if (!strcmp(argv[i], "--squeeze-blank"))
                flags->s_flag = 1;
            else {
                for (size_t j = 1; j < strlen(argv[i]); j++) {
                    if (argv[i][j] == 'b')
                        flags->b_flag = 1;
                    else if (argv[i][j] == 'v')
                        flags->v_flag = 1;  // just for test
                    else if (argv[i][j] == 'e')
                        flags->e_flag = flags->v_flag = 1;
                    else if (argv[i][j] == 'E')
                        flags->e_flag = 1;
                    else if (argv[i][j] == 'n')
                        flags->n_flag = 1;
                    else if (argv[i][j] == 's')
                        flags->s_flag = 1;
                    else if (argv[i][j] == 't')
                        flags->v_flag = flags->t_flag = 1;
                    else if (argv[i][j] == 'T')
                        flags->t_flag = 1;
                    else {
                        fprintf(stderr, "Illegal Options!\n");
                        exit(EXIT_FAILURE);
                    }
                    if (flags->b_flag == 1) flags->n_flag = 0;
                }
            }
        } else {
            start_files = i;
            is_flags_end = 1;
        }
    }
    return start_files;
}

void show_files(int start_files, int argc, char **argv, flags_struct flags) {
    for (int i = start_files; i < argc; ++i) {
        print_file(argv[i], flags);
    }
}

_Bool print_file(char *filename, flags_struct flags) {
    _Bool is_error = 0;
    int count_nums = 1;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Can't open file!\n");
        is_error = 1;
    }

    if (!is_error) {
        int next_ch = '\0';
        int curr_ch = fgetc(fp);

        if (flags.n_flag || (flags.b_flag && curr_ch != '\n')) {
            fprintf(stdout, "%6d\t", count_nums++);  // start
        }

        int n_count_buffer = 0;

        if (curr_ch != EOF) {
            while ((curr_ch != EOF)) {  // s flag is work here
                next_ch = fgetc(fp);

                if (flags.s_flag && curr_ch == '\n' && next_ch == '\n') {  // s flag
                    if (++n_count_buffer > 1) {
                        continue;
                    }
                } else {
                    n_count_buffer = 0;
                }

                count_nums = print_element(curr_ch, next_ch, count_nums, flags);

                curr_ch = next_ch;
            }
        } else {
            is_error = 1;
        }
        int closing = fclose(fp);
        if (closing) {
            fprintf(stderr, "Can't close file!\n");
            is_error = 1;
        }
    }
    return is_error;
}

int print_element(int curr_ch, int next_ch, int count_nums,
                  flags_struct flags) {
    _Bool is_invisible = 0;
    if (flags.t_flag || flags.v_flag || flags.e_flag) {
        if (flags.v_flag) {
            if (curr_ch >= 0 && curr_ch < 32 && curr_ch != '\n' && curr_ch != '\t') {
                fprintf(stdout, "^%c", curr_ch + 64);
                is_invisible = 1;
            } else if (curr_ch >= 127) {
                fprintf(stdout, "^%c", curr_ch - 64);
                is_invisible = 1;
            }
        }
        if (flags.t_flag && curr_ch == '\t') {
            fprintf(stdout, "^I");
            is_invisible = 1;
        }
        if (flags.e_flag && curr_ch == '\n') {
            putc('$', stdout);
        }
    }

    if (!is_invisible) {
        putc(curr_ch, stdout);
    }

    if (((flags.n_flag && curr_ch == '\n') ||  // all lines
         (flags.b_flag && curr_ch == '\n' && next_ch != '\n')) &&
        (next_ch != EOF)) {  // not empty lines
        fprintf(stdout, "%6d\t", count_nums++);
    }
    return count_nums;
}
