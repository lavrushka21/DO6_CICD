#!/bin/bash
set -e

echo "Копируем бинарники на продакшн-сервер..."
scp -o StrictHostKeyChecking=no src/cat/s21_cat src/grep/s21_grep student@192.168.64.3:/tmp/

echo "Перемещаем в /usr/local/bin и даем права..."
ssh -o StrictHostKeyChecking=no student@192.168.64.3 "sudo mv /tmp/s21_cat /usr/local/bin/ && sudo mv /tmp/s21_grep /usr/local/bin/ && sudo chmod +x /usr/local/bin/s21_cat /usr/local/bin/s21_grep"

echo "Деплой прошел успешно!"