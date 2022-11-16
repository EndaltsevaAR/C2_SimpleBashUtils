WWW = -Wall -Werror -Wextra

all: clean prepare test_1 test_2 test_3 clean

#build and copy temporary files
prepare:
	gcc $(WWW) ../cat/s21_cat.c -o s21_cat
	cp ../cat/s21_cat.c s21_cat.c
	cp ../cat/s21_cat.h s21_cat.h
	cp ../../materials/linters/.clang-format .clang-format

#test_clang with .clang-format
test_1:
	clang-format -n s21_cat.c
	clang-format -n s21_cat.h

#functional test
test_2:
	sh test_func_cat.sh

#leak test
test_3:
	sh test_leak_cat.sh

clean:
	rm -rf s21_cat.* s21_cat *log .clang-format

#valgrind test for linux only
test_valgrind:
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./s21_cat -b -v -t -n -s -e test_case_cat.txt

