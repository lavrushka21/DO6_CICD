#!/bin/bash

GREP_DIR="../grep"
FILE1="$GREP_DIR/file.txt"
FILE2="$GREP_DIR/file1.txt"
MY_GREP="$GREP_DIR/s21_grep"
REAL_GREP="grep"
FAILED=0

echo "=== Запуск тестов для s21_grep ==="

# Тест -e
echo "Testing grep -e..."
$REAL_GREP -e "Connection" $FILE1 $FILE2 > grep_out 2>/dev/null
$MY_GREP -e "Connection" $FILE1 $FILE2 > s21_grep_out 2>/dev/null
if diff grep_out s21_grep_out > /dev/null; then echo "grep -e OK"; else echo "grep -e FAIL"; FAILED=1; fi

# Тест -i
echo "Testing grep -i..."
$REAL_GREP -e "DATABASE" -i $FILE1 $FILE2 > grep_out 2>/dev/null
$MY_GREP -e "DATABASE" -i $FILE1 $FILE2 > s21_grep_out 2>/dev/null
if diff grep_out s21_grep_out > /dev/null; then echo "grep -i OK"; else echo "grep -i FAIL"; FAILED=1; fi

# Тест -c
echo "Testing grep -c..."
$REAL_GREP -c "[0-9]" $FILE1 $FILE2 > grep_out 2>/dev/null
$MY_GREP -c "[0-9]" $FILE1 $FILE2 > s21_grep_out 2>/dev/null
if diff grep_out s21_grep_out > /dev/null; then echo "grep -c OK"; else echo "grep -c FAIL"; FAILED=1; fi

# Тест -l
echo "Testing grep -l..."
$REAL_GREP -l "RAM" $FILE1 $FILE2 > grep_out 2>/dev/null
$MY_GREP -l "RAM" $FILE1 $FILE2 > s21_grep_out 2>/dev/null
if diff grep_out s21_grep_out > /dev/null; then echo "grep -l OK"; else echo "grep -l FAIL"; FAILED=1; fi

# Тест -n
echo "Testing grep -n..."
$REAL_GREP -n "batch" $FILE1 $FILE2 > grep_out 2>/dev/null
$MY_GREP -n "batch" $FILE1 $FILE2 > s21_grep_out 2>/dev/null
if diff grep_out s21_grep_out > /dev/null; then echo "grep -n OK"; else echo "grep -n FAIL"; FAILED=1; fi

# Тест -v
echo "Testing grep -v..."
$REAL_GREP -v "Connection" $FILE1 $FILE2 > grep_out 2>/dev/null
$MY_GREP -v "Connection" $FILE1 $FILE2 > s21_grep_out 2>/dev/null
if diff grep_out s21_grep_out > /dev/null; then echo "grep -v OK"; else echo "grep -v FAIL"; FAILED=1; fi

rm -f grep_out s21_grep_out

if [ $FAILED -eq 1 ]; then
    echo "!!! Тесты grep ПРОВАЛЕНЫ !!!"
    exit 1
else
    echo "Все тесты grep ПРОШЛИ УСПЕШНО!"
    exit 0
fi