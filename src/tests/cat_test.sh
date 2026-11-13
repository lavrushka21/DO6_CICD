#!/bin/bash

# Определяем пути относительно папки src/tests
CAT_DIR="../cat"
FILE1="$CAT_DIR/file1.txt"
FILE2="$CAT_DIR/file2.txt"
MY_CAT="$CAT_DIR/s21_cat"
REAL_CAT="cat"
FLAGS_SHORT="b e n s t T E v"
FLAGS_LONG="--number-nonblank --number --squeeze-blank"
FAILED=0

echo "=== Запуск тестов для s21_cat ==="

for flag in $FLAGS_SHORT; do
    echo "Testing -$flag flag:"
    $REAL_CAT -$flag $FILE1 $FILE2 > real_cat_out 2>/dev/null
    $MY_CAT -$flag $FILE1 $FILE2 > my_cat_out 2>/dev/null
    
    if diff real_cat_out my_cat_out > /dev/null; then
        echo "Test -$flag OK"
    else
        echo "Test -$flag FAIL"
        FAILED=1
    fi
done

for flag in $FLAGS_LONG; do
    echo "Testing $flag:"
    $REAL_CAT $flag "$FILE1" "$FILE2" > real_cat_out 2>/dev/null
    $MY_CAT $flag "$FILE1" "$FILE2" > my_cat_out 2>/dev/null
    
    if diff real_cat_out my_cat_out > /dev/null; then
        echo "Test $flag OK"
    else
        echo "Test $flag FAIL"
        FAILED=1
    fi
done

rm -f real_cat_out my_cat_out

if [ $FAILED -eq 1 ]; then
    echo "!!! Тесты cat ПРОВАЛЕНЫ !!!"
    exit 1
else
    echo "Все тесты cat ПРОШЛИ УСПЕШНО!"
    exit 0
fi