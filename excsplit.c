#include <stdio.h>
#include "defs.h"
#include "f256.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>

int fp_vreader(int i, FILE *fps[]) {
  return fgetc(fps[i]);
}

int fp_vwriter(int i, int c, FILE *fps[]) {
  return putc(c, fps[i]);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  if (argc <= 3) {
    fprintf(stderr,
            "Split a file, which can be reconstructed by any k files.\n\n"
            "Usage: %s [+|-]k whole-file split-files-list[must >=k]\n\n"
            " k:\ta interger[1-256],"
            " k files needed to reconstruct original whole-file,\n"
            "   \tany (k-1) files can not reconstruct original whole-file.\n"
            "   \t+k means split, -k means gather.\n\n"
            "Not well tested, No warranty, use it at you own risk!\n"
            "Author: jiangzuoyan@gmail.com\n"
            "Date: 2009-11-26\n",
            argv[0]);
    exit(1);
  }
  int k = atoi(argv[1]), n, i, j;
  if (k == 0 || abs(k) > 256) {
    fprintf(stderr, "invalid k\n");
    exit(10);
  }
  if (k > 0) {
    n = argc - 3;
    if (n < k) {
      fprintf(stderr, "no enough args\n");
      exit(11);
    }
    FILE *fin = stdin;
    if (strcmp(argv[2], "-")) {
      fin = fopen(argv[2], "rb");
      if (fin == NULL) {
        fprintf(stderr, "open read file %s failed:%s\n", argv[2], strerror(errno));
        exit(12);
      }
    }
    FILE *fouts[n];
    for (i = 0; i < n; ++i) {
      fouts[i] = fopen(argv[i + 3], "wxb");
      if (fouts[i] == NULL) {
        fprintf(stderr, "open write file %s failed:%s\n", argv[i + 3], strerror(errno));
        exit(13);
      }
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
    gpoly_encrypt(k, n, fgetc, fin, v, fp_vwriter, fouts);
  } else {
    k = - k;
    if (argc < k + 3) {
      fprintf(stderr, "no enough args\n");
      exit(14);
    }
    FILE *fout = stdout;
    if (strcmp(argv[2], "-")) {
      fout = fopen(argv[2], "wxb");
      if (fout == NULL) {
        fprintf(stderr, "open write file %s failed:%s\n", argv[2], strerror(errno));
        exit(15);
      }
    }
    FILE *fins[k];
    for (i = 0; i < k; ++i) {
      fins[i] = fopen(argv[3 + i], "rb");
      if (fins[i] == NULL) {
        fprintf(stderr, "open read file %s failed:%s\n", argv[i + 3], strerror(errno));
        exit(16);
      }
    }
    unsigned char v[k];
    gpoly_decrypt(k, k, fp_vreader, fins, v, putc, fout);
  }
  return 0;
}
