#include <stdio.h>
#include "defs.h"
#include "f256.h"

int fp_vreader(int i, FILE *fps[]) {
  return fgetc(fps[i]);
}

int fp_vwriter(int i, int c, FILE *fps[]) {
  return putc(c, fps[i]);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  if (argc <= 2) exit(1);
  int k = atoi(argv[1]), n, i, j;
  if (k > 0) {
    n = argc - 2;
    FILE *fouts[n];
    for (i = 2; i < argc; ++i) {
      fouts[i - 2] = fopen(argv[i], "w");
      if (fouts[i - 2] == NULL) exit(13);
    }
    unsigned char v[n];
    for (i = 0; i < n; ++i) {
      do {
        v[i] = random();
        for (j = MAX(0, i - 128); j < i; ++j) {
          if (v[i] == v[j]) break;
        }
      } while (j < i);
    }
    gpoly_encrypt(k, n, fgetc, stdin, fp_vwriter, fouts);
  } else {
    k = - k;
    FILE *fins[k];
    for (i = 0; i < k; ++i) {
      fins[i] = fopen(argv[2 + i], "r");
      if (fins[i] == NULL) exit(2);
    }
    unsigned char v[k];
    gpoly_decrypt(k, k, fp_vreader, fins, putc, stdout);
  }
  return 0;
}
