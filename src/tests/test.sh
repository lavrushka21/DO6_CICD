#!/bin/bash

echo "Запуск интеграционных тестов для DO..."

# Тест 1: Проверка вывода для числа 1
output=$(./DO 1)
if [ "$output" != "Learning to Linux" ]; then
  echo "Тест 1 ПРОВАЛЕН: ожидалось 'Learning to Linux', получено '$output'"
  exit 1
fi

# Тест 2: Проверка вывода для числа 6
output=$(./DO 6)
if [ "$output" != "Learning to CI/CD" ]; then
  echo "Тест 2 ПРОВАЛЕН: ожидалось 'Learning to CI/CD', получено '$output'"
  exit 1
fi

# Тест 3: Проверка неверного количества аргументов
output=$(./DO 2>&1)
if [ "$output" != "Bad number of arguments!" ]; then
  echo "Тест 3 ПРОВАЛЕН: ожидалось 'Bad number of arguments!', получено '$output'"
  exit 1
fi

echo "Все интеграционные тесты ПРОШЛИ УСПЕШНО!"
exit 0