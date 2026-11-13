#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int E;
  int T;
  int v;
} Options;

struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                {"number", 0, 0, 'n'},
                                {"squeeze-blank", 0, 0, 's'},
                                {0, 0, 0, 0}};

void cat(int argc, char *argv[]);
void openFile(char *filename, Options *flags);
void parserFlags(FILE *file, Options *flags);
void ReadFlags(int argc, char *argv[], Options *flags, int *stop_cat);
void operation_b(int symbol, int *str_count, int *prev_char);
void operation_E(int symbol);
void operation_e(int symbol);
void operation_n(int symbol, int *str_count, int *numb_count);
void operation_s(int symbol, int *last_symbol, int *empty_line);
void operation_t(int symbol);
void operation_T(int symbol);
void operation_v(int symbol);

int main(int argc, char *argv[]) {
  cat(argc, argv);
  return 0;
}

void cat(int argc, char *argv[]) {
  Options flags = {0};
  int stop_cat = 0;
  ReadFlags(argc, argv, &flags, &stop_cat);

  if (!stop_cat && optind < argc) {
    for (int i = optind; i < argc; i++) {
      openFile(argv[i], &flags);
    }
  }
}

void openFile(char *filename, Options *flags) {
  FILE *file = fopen(filename, "rt");
  if (file != NULL) {
    parserFlags(file, flags);
    fclose(file);
  } else {
    printf("No such file: %s\n", filename);
  }
}

void ReadFlags(int argc, char *argv[], Options *flags, int *stop_cat) {
  int res;
  while ((res = getopt_long(argc, argv, "benstETv", long_options, NULL)) !=
         -1) {
    switch (res) {
      case 'b':
        flags->b = 1;
        break;
      case 'e':
        flags->e = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->t = 1;
        break;
      case 'E':
        flags->E = 1;
        break;
      case 'T':
        flags->T = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case '?':
        printf("Error\n");
        *stop_cat = 1;
        break;
    }
  }
}

void parserFlags(FILE *file, Options *flags) {
  int symbol;
  int str_count = 1;
  int numb_count = 0;
  int last_symbol = '\n';
  int empty_line = 0;
  int prev_char = '\n';

  while ((symbol = fgetc(file)) != EOF) {
    if (!flags->b && !flags->e && !flags->n && !flags->s && !flags->t &&
        !flags->E && !flags->T && !flags->v) {
      putchar(symbol);
    } else {
      if (flags->s) {
        operation_s(symbol, &last_symbol, &empty_line);
      } else if (flags->b) {
        operation_b(symbol, &str_count, &prev_char);
      } else if (flags->n) {
        operation_n(symbol, &str_count, &numb_count);
      } else if (flags->t) {
        operation_t(symbol);
      } else if (flags->T) {
        operation_T(symbol);
      } else if (flags->E) {
        operation_E(symbol);
      } else if (flags->v && !flags->e && !flags->t) {
        operation_v(symbol);
      } else if (flags->e) {
        operation_e(symbol);
      }
    }
  }
}

void operation_b(int symbol, int *str_count, int *prev_char) {
  if (*prev_char == '\n' && symbol != '\n') {
    printf("%6d\t", (*str_count)++);
  }
  putchar(symbol);
  *prev_char = symbol;
}

void operation_E(int symbol) {
  if (symbol == '\n') {
    printf("$");
  }
  putchar(symbol);
}

void operation_n(int symbol, int *str_count, int *numb_count) {
  if (*numb_count < *str_count) {
    printf("%6d\t", *str_count);
    (*numb_count)++;
  }
  putchar(symbol);
  if (symbol == '\n') {
    (*str_count)++;
  }
}

void operation_s(int symbol, int *last_symbol, int *empty_line) {
  if (*last_symbol == '\n' && symbol == '\n') {
    (*empty_line)++;
    if (*empty_line == 1) {
      putchar('\n');
    }
  } else {
    putchar(symbol);
    *empty_line = 0;
  }

  *last_symbol = symbol;
}

void operation_t(int symbol) {
  if (symbol == '\t') {
    printf("^I");
  } else {
    operation_v(symbol);
  }
}

void operation_T(int symbol) {
  if (symbol == '\t') {
    printf("^I");
  } else {
    putchar(symbol);
  }
}

void operation_v(int symbol) {
  if (symbol == 127) {
    printf("^?");
  } else if (symbol > 127 && symbol < 160) {
    printf("M-^%c", symbol - 64);
  } else if (symbol >= 160) {
    printf("M-%c", symbol - 128);
  } else if ((symbol < 32) && (symbol != '\n') && (symbol != '\t')) {
    printf("^%c", symbol + 64);
  } else {
    putchar(symbol);
  }
}

void operation_e(int symbol) {
  if (symbol == '\n') {
    printf("$");
    putc(symbol, stdout);
  } else {
    operation_v(symbol);
  }
}