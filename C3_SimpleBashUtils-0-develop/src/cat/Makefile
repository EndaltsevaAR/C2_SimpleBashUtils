CC: gcc
CFLAGS: -Wall -Werror -Wextra

all: s21_cat

s21_cat: s21_cat.c
	$(CC) $(CFLAGS) -c s21_cat.c
	$(CC) s21_cat.o -o s21_cat

clean:
	rm -rf *.o
	rm -rf s21_cat

rebuild: clean all

