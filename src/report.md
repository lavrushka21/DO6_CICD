## Part 1. Настройка gitlab-runner

Для выполнения задания была развернута виртуальная машина с операционной системой **Ubuntu Server 24.04 LTS (ARM64)** с использованием виртуализации **UTM** (движок QEMU). Сеть настроена в режиме Shared Network, виртуальная машина получила IP-адрес `192.168.64.2`.

На виртуальную машину установлен и настроен **gitlab-runner** с исполнителем `shell`. Регистрация выполнена для текущего проекта (DO6_CICD). Runner успешно подключен к GitLab и находится в статусе online.

- Проверка версии ОС:
  ![Версия ОС](screenshots/part1_os_release.png)

- IP-адрес виртуальной машины:
  ![IP виртуальной машины](screenshots/part1_vm_ip.png)

- Проверка установленной версии gitlab-runner:
  ![Версия gitlab-runner](screenshots/part1_runner_version.png)

- Статус службы gitlab-runner в системе:
  ![Статус службы](screenshots/part1_runner_status.png)

- Статус runner в GitLab (online):
  ![Runner online](screenshots/part1_runner_online.png)


## Part 2. Сборка

В файл `.github/workflows/ci.yml` добавлен этап `build`, который автоматически запускает сборку приложения из папки `code-samples` (утилита DO). Артефакт сборки (бинарник `DO`) сохраняется в GitHub на 30 дней.

Пайплайн успешно проходит этап сборки.

- Успешный пайплайн со стадией сборки:
  ![Pipeline success](screenshots/part2_github_pipeline.png)

- Лог выполнения сборки (компиляция без ошибок):
  ![Job log](screenshots/part2_github_job_log.png)

- Сохраненные артефакты (бинарник DO):
  ![Artifacts](screenshots/part2_github_artifacts.png)
  

## Part 3. Тест кодстайла

В файл `.github/workflows/ci.yml` добавлен этап `style`, который запускает проверку форматирования кода с помощью утилиты `clang-format`. Этап зависит от успешной сборки (`needs: build`), поэтому запускается только после неё.

Если код не соответствует стандартам, утилита возвращает ненулевой код выхода, и пайплайн завершается с ошибкой (красный статус).

- Успешный пайплайн со стадиями сборки и стиля:
  ![Pipeline success](screenshots/part3_github_pipeline.png)

- Лог проверки кодстайла:
  ![Style log](screenshots/part3_github_style_log.png)


## Part 4. Интеграционные тесты

В файл `.github/workflows/ci.yml` добавлен этап `integration`, который запускает интеграционные тесты. Тесты написаны на языке Bash и находятся в файле `src/tests/test.sh`. Они вызывают собранное приложение `DO` с разными аргументами и проверяют корректность его вывода.

Этап запускается автоматически только при условии успешного прохождения этапов сборки и кодстайла (`needs: [build, style]`). Если тесты не проходят, скрипт возвращает код ошибки, и пайплайн «фейлится» (становится красным).

- Успешный пайплайн со всеми тремя стадиями (build, style, integration):
  ![Pipeline success](screenshots/part4_github_pipeline.png)

- Лог успешного прохождения интеграционных тестов:
  ![Tests log](screenshots/part4_github_tests_log.png)

# Part 5. Этап деплоя

Для выполнения этапа CD была поднята **вторая виртуальная машина** Ubuntu Server 24.04 LTS (production) с IP-адресом `192.168.64.3`. 

**Настройка окружения (без скриншотов):**
1.  На первой машине (runner) был сгенерирован SSH-ключ (`ssh-keygen`).
2.  Публичный ключ был скопирован на вторую машину в файл `~/.ssh/authorized_keys` для обеспечения беспарольного доступа.
3.  На второй машине был настроен `sudoers` (через `visudo`), чтобы пользователь `student` мог выполнять команды `mv` и `chmod` в `/usr/local/bin` без ввода пароля (необходимо для работы CI/CD).

Написан bash-скрипт `deploy.sh`, который с помощью `scp` копирует собранный бинарник `DO` на вторую виртуальную машину, а с помощью `ssh` перемещает его в директорию `/usr/local/bin` и выдает права на выполнение.

В файл `.github/workflows/ci.yml` добавлен этап `deploy`, который запускается только после успешного прохождения всех предыдущих этапов (`needs: [build, style, integration]`).

- Успешный пайплайн с этапом деплоя:
  ![Pipeline success](screenshots/part5_github_pipeline.png)

- Лог успешного деплоя (скрипт отработал):
  ![Deploy log](screenshots/part5_github_deploy_log.png)

- Проверка наличия бинарника на production-сервере (права на выполнение):
  ![Binaries on prod](screenshots/part5_binaries_on_prod.png)