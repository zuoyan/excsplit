#ifndef FILE_F256_H__
#define FILE_F256_H__ 1

unsigned char gpoly_eval(int n, unsigned char *a, unsigned char x);
unsigned char gpoly_interp(int k, unsigned char *xs, unsigned char *ys, unsigned char x);
void gpoly_rev(int k, const unsigned char *xs, const unsigned char *ys, unsigned char *P);

#endif
