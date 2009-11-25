#ifndef FILE_F256_H__
#define FILE_F256_H__ 1

// polynomial x8 + x4 + x3 + x + 1.
static inline unsigned char gadd(unsigned char a, unsigned char b) {
  return a ^ b;
}

static inline unsigned char gsub(unsigned char a, unsigned char b) {
  return a ^ b;
}

static inline void print_cvector(FILE *fp,const char *pre, const char *post, int n,
                  const unsigned char *A, int inc) {
  fprintf(fp, "%s", pre);
  int i;
  for (i = 0; i < n; ++i) {
    fprintf(fp, "%s0x%02x", i == 0 ? "" : ",", A[i * inc]);
  }
  fprintf(fp, "%s", post);
}

unsigned char gmul(unsigned char a, unsigned char b);
unsigned char ginv(unsigned char in);
unsigned char gdiv(unsigned char n, unsigned char q);
void poly_randoms(unsigned char *x, int n);
unsigned char poly_eval(int n, unsigned char *a, unsigned char x);
unsigned char poly_interp(int k, unsigned char *xs, unsigned char *ys, unsigned char x);
void poly_rev(int k, const unsigned char *xs, const unsigned char *ys, unsigned char *P);
void scatter(int k, int n, FILE *fin, FILE **fouts);
void gather(int k, FILE **fins, FILE *fout);

#endif
