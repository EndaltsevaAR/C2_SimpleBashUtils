#ifndef SRC_GREP_S21_CAT_H_
#define SRC_GREP_S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
};

typedef struct grep_flags grep_flags_struct;

#endif  // SRC_GREP_S21_CAT_H_