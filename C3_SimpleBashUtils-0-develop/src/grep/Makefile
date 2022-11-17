CC: gcc
CFLAGS: -Wall -Werror -Wextra

all: s21_grep

s21_cat: s21_grep.c
	$(CC) $(CFLAGS) -c s21_grep.c
	$(CC) s21_grep.o -o s21_grep

clean:
	rm -rf *.o
	rm -rf s21_grep

rebuild: clean all