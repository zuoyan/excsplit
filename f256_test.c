#include <stdio.h>
#include "defs.h"
#include "f256.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int test_poly_rev(int k) {
  int i;
  unsigned char xs[k];
  unsigned char ys[k];
  unsigned char P[k], cP[k];
  for (i = 0; i < k; ++i) {
    xs[i] = i + 152;
    P[i] = random();
  }
  print_cvector(stdout, "X=[", "]\n", k, xs, 1);
  print_cvector(stdout, "P=[", "]\n", k, P, 1);
  for (i = 0; i < k; ++i) {
    ys[i] = poly_eval(k, P, xs[i]);
  }
  print_cvector(stdout, "Y=[", "]\n", k, ys, 1);
  for (i = 0; i < k; ++i) {
    mesg_assert(poly_interp(k, xs, ys, xs[i]) == ys[i], "x=%d y=%d i=%d", xs[i], ys[i], i);
    assert(poly_eval(k, P, xs[i]) == ys[i]);
  }
  for (i = 0; i < 1000; ++i) {
    unsigned char x = random();
    mesg_assert(poly_interp(k, xs, ys, x) == poly_eval(k, P, x), "x=0x%02x", (int)x);
  }
  poly_rev(k, xs, ys, cP);
  print_cvector(stdout, "cP=[", "]\n", k, cP, 1);
  for (i = 0; i < k; ++i) {
    if (cP[i] != P[i]) return -1;
  }
  trace_line("test_poly_rev succeeded for k=%d", k);
  return 0;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  trace_line("0x%02x", (int)gadd(0, 0x30));
  trace_line("0x%02x", (int)gmul(1, 0x30));
  trace_line("0x%02x", (int)gdiv(0x30, 1));
  int ret, i, n = 300;
  for (i = 256; i < n; ++i) {
    ret = test_poly_rev(i);
    mesg_assert(ret >= 0, "poly_rev failed at k=%d", i);
  }
  return 0;
}
