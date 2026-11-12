#!/bin/bash
set -e

echo "Копируем бинарник DO на продакшн-сервер..."
scp -o StrictHostKeyChecking=no code-samples/DO student@192.168.64.3:/tmp/DO

echo "Перемещаем в /usr/local/bin и даем права..."
ssh -o StrictHostKeyChecking=no student@192.168.64.3 "sudo mv /tmp/DO /usr/local/bin/DO && sudo chmod +x /usr/local/bin/DO"

echo "Деплой прошел успешно!"