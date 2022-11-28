WWW = -Wall -Werror -Wextra -g

all: clean prepare test_1 test_2 test_3 test_valgrind clean

#build and copy temporary files
prepare:
	gcc $(WWW) ../grep/s21_grep.c -o s21_grep
	cp ../grep/s21_grep.c s21_grep.c
	cp ../grep/s21_grep.h s21_grep.h
	cp ../../materials/linters/.clang-format .clang-format

#test_clang with .clang-format
test_1:
	clang-format -n s21_grep.c
	clang-format -n s21_grep.h

#functional test
test_2:
	sh test_func_grep.sh

#leak test
test_3:
	sh test_leak_grep.sh

clean:
	rm -rf s21_grep.* s21_grep *log .clang-format

#valgrind test for linux only
test_valgrind:
	bash test_ERROR_VALGRIND.sh