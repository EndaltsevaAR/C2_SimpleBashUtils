#ifndef SRC_GREP_S21_CAT_H_
#define SRC_GREP_S21_CAT_H_
#define BUFFER_IN_FILE 512
#define SIZE_FOR_TEMPLATE 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>


struct grep_flags {
    _Bool e_flag;    // шаблон
    _Bool f_flag;    // файл с шаблонами
    _Bool v_flag;    // инвертирует смысл поиска соответствий
    _Bool c_flag;    // выводит количество строк с совпадениями
    _Bool l_flag;    // выводит названия файлов с совпадениями
    _Bool n_flag;    // предваряет каждую строку вывода номером строки из файла
    _Bool i_flag;    // игнорирует различия регистра
    _Bool o_flag;    // печатает только совпадающие (непустые) части строки через \n
    _Bool h_flag;    // не будет выводить название файла
    _Bool s_flag;
};

typedef struct TemplateNode {
    char template_text[SIZE_FOR_TEMPLATE];
    struct TemplateNode *next;
} Template;

typedef struct grep_flags grep_flags_struct;

_Bool grep_function(int argc, char *argv[]);
bool parser(int argc, char *argv[], grep_flags_struct *flags, int *files_argv, Template **template_head, size_t *files_count);
Template *create_node(char *value);
void add_template_to_end_list(Template *head, char *value);
void add_templates_from_file(Template **head, char *value);
void free_template_node(Template *head);
void shift_files_array(int *files_argv);
void files_processing(Template *template_node, grep_flags_struct flags, const int *files_argv, char *argv[], size_t files_argv_size);
void common_processing(FILE *fp, grep_flags_struct flags, Template *template_node, char *filename, size_t files_argv_size);
int compare_word_patterns(char *buffer, grep_flags_struct flags, Template *template_node);

#endif  // SRC_GREP_S21_CAT_H_
