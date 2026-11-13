CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: s21_cat

c21_cat: s21_cat.o
	$(CC) $(CFLAGS) s21_cat.o -o s21_cat

c21_cat.o: s21_cat.c
	$(CC) $(CFLAGS) -c s21_cat.c -o s21_cat.o

clean:
	rm -f *.o s21_cat
