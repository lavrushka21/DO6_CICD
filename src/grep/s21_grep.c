#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATTERNS 4096
#define BUFFER_SIZE 4096

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  char *patterns[MAX_PATTERNS];
  int pattern_count;
} Options;

void grep(int argc, char *argv[]);
void readFlags(int argc, char *argv[], Options *flags, int *stop_grep);
void openFile(char *filename, Options *flags, int *stop_grep, int count_files);
void processingFile(FILE *file, Options *flags, char *filename, int *stop_grep,
                    int count_files);
void parserFlags(FILE *file, Options *flags, regex_t *regex, char *filename,
                 int count_files);
void basicOutput(FILE *file, regex_t *regex, char *filename, int count_files);
void operation_v(FILE *file, regex_t *regex, char *filename, int count_files);
void operation_c(FILE *file, regex_t *regex, char *filename, int count_files);
void operation_l(regex_t *regex, char *filename, FILE *file);
void operation_n(FILE *file, regex_t *regex, char *filename, int count_files);

int main(int argc, char *argv[]) {
  grep(argc, argv);
  return 0;
}

void grep(int argc, char *argv[]) {
  Options flags = {0};
  int stop_grep = 0;
  readFlags(argc, argv, &flags, &stop_grep);

  int count_files = (argc - optind > 1);

  if (stop_grep != 1 && optind < argc) {
    for (int i = optind; i < argc; i++) {
      openFile(argv[i], &flags, &stop_grep, count_files);
    }
  }
}

void readFlags(int argc, char *argv[], Options *flags, int *stop_grep) {
  int rez;
  while ((rez = getopt(argc, argv, "e:ivcln")) != -1) {
    switch (rez) {
      case 'e':
        if (flags->pattern_count < MAX_PATTERNS) {
          flags->patterns[flags->pattern_count++] = optarg;
          flags->e = 1;
        } else {
          printf("too many patterns\n");
          *stop_grep = 1;
        }
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case '?':
        printf("Error\n");
        *stop_grep = 1;
        break;
    }
  }
  if (flags->pattern_count == 0 && optind < argc) {
    flags->patterns[0] = argv[optind];
    flags->pattern_count = 1;
    optind++;
  }
}

void openFile(char *filename, Options *flags, int *stop_grep, int count_files) {
  FILE *file = fopen(filename, "r");
  if (file != NULL) {
    static int file_index = 0;
    file_index++;
    processingFile(file, flags, filename, stop_grep, count_files);
    fclose(file);
  } else {
    printf("No such file\n");
    *stop_grep = 1;
  }
}

void processingFile(FILE *file, Options *flags, char *filename, int *stop_grep,
                    int count_files) {
  regex_t regex;
  int compiled_flags = flags->i ? REG_ICASE : 0;

  if (regcomp(&regex, flags->patterns[0], REG_EXTENDED | compiled_flags) != 0) {
    printf("error complation\n");
    *stop_grep = 1;
  }

  if (*stop_grep != 1) {
    parserFlags(file, flags, &regex, filename, count_files);
  }
  regfree(&regex);
}

void parserFlags(FILE *file, Options *flags, regex_t *regex, char *filename,
                 int count_files) {
  if (flags->l) {
    operation_l(regex, filename, file);
  } else if (flags->v) {
    operation_v(file, regex, filename, count_files);
  } else if (flags->c) {
    operation_c(file, regex, filename, count_files);
  } else if (flags->n) {
    operation_n(file, regex, filename, count_files);
  } else {
    basicOutput(file, regex, filename, count_files);
  }
}

void basicOutput(FILE *file, regex_t *regex, char *filename, int count_files) {
  char line[BUFFER_SIZE];
  rewind(file);
  while (fgets(line, BUFFER_SIZE, file)) {
    if (regexec(regex, line, 0, 0, 0) == 0) {
      if (count_files) {
        printf("%s:%s", filename, line);
      } else {
        printf("%s", line);
      }
    }
  }
}

void operation_v(FILE *file, regex_t *regex, char *filename, int count_files) {
  char line[BUFFER_SIZE];
  while (fgets(line, BUFFER_SIZE, file)) {
    if (regexec(regex, line, 0, 0, 0) != 0) {
      line[strcspn(line, "\n")] = '\0';
      if (count_files) {
        printf("%s:%s\n", filename, line);
      } else {
        printf("%s\n", line);
      }
    }
  }
}

void operation_c(FILE *file, regex_t *regex, char *filename, int count_files) {
  char line[BUFFER_SIZE];
  int count_same = 0;
  while (fgets(line, BUFFER_SIZE, file)) {
    if (regexec(regex, line, 0, 0, 0) == 0) {
      count_same++;
    }
  }
  if (count_files) {
    printf("%s:%d\n", filename, count_same);
  } else {
    printf("%d\n", count_same);
  }
}

void operation_l(regex_t *regex, char *filename, FILE *file) {
  int index_found = 0;
  char line[BUFFER_SIZE];
  rewind(file);
  while (index_found != 1 && fgets(line, BUFFER_SIZE, file)) {
    if (regexec(regex, line, 0, 0, 0) == 0) {
      index_found = 1;
    }
  }
  if (index_found == 1) {
    printf("%s\n", filename);
  }
}

void operation_n(FILE *file, regex_t *regex, char *filename, int count_files) {
  char line[BUFFER_SIZE];
  int count_line = 1;
  rewind(file);
  while (fgets(line, BUFFER_SIZE, file)) {
    if (regexec(regex, line, 0, 0, 0) == 0) {
      if (count_files) {
        printf("%s:%d:%s", filename, count_line, line);
      } else {
        printf("%d:%s", count_line, line);
      }
    }
    count_line++;
  }
}
