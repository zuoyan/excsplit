#ifndef FILE_F256_H__
#define FILE_F256_H__ 1

unsigned char gpoly_eval(int n, unsigned char *a, unsigned char x);
unsigned char gpoly_interp(int k, unsigned char *xs, unsigned char *ys, unsigned char x);
void gpoly_rev(int k, const unsigned char *xs, const unsigned char *ys, unsigned char *P);
void gpoly_encrypt_iv(int k, int n, int len, unsigned char *in, unsigned char v[], unsigned char *outs[]);
void gpoly_decrypt_iv(int k, int n, int len, unsigned char *ins[], unsigned char v[], unsigned char *out);
int gpoly_encrypt(int k, int n, int (*reader)(void *d), void *rdata, unsigned char v[ ], int (*vwriter)(int i, int c, void *d), void *wdata);
int gpoly_decrypt(int k, int n, int (*vreader)(int i, void *d), void *rdata, unsigned char v[ ], int (*writer)(int c, void *d), void *wdata);

#endif
