#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cat_flags {
  _Bool b_flag;
  _Bool e_flag;
  _Bool n_flag;
  _Bool s_flag;
  _Bool t_flag;
  _Bool v_flag;
};

typedef struct cat_flags flags_struct;

_Bool cat_function(int argc, char *argv[]);
_Bool print_file(char *filename, flags_struct flags);
int do_parser(int argc, char **argv, flags_struct *flags);
void show_files(int start_files, int argc, char **argv, flags_struct flags);
int print_element(int curr_ch, int next_ch, int count_nums, flags_struct flags);

#endif  // SRC_CAT_S21_CAT_H_
