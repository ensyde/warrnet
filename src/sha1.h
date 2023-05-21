#ifndef SHA1_H
#define SHA1_H

#include "types.h"

// #define ROTL(x,n,w) (((x)<<((n)&(w-1))) | ((x)>>(((-(n))&(w-1)))))
#define ROTL(x,n,w) (((n)%(w)) ? (((x)<<((n)%(w))) | ((x)>>((w)-((n)%(w))))) : (x))
#define ROL(x,n) ROTL(x,n,32)

void sha1_hash( unsigned char *src, int len, unsigned char *dest);
void GetPasswdHash(u32 Seed, u32 SessionKey, u8 *pwHash, u8 *Out, bool DumpVars);
void DumpHash(u8 *Hash, int Size);

#endif
