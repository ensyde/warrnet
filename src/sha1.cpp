#include <string.h>
#include <stdio.h>

#include "sha1.h"

#include <stdio.h>

// Creates a 5*4 bytes hash and puts it in dest.
void sha1_hash( unsigned char *src, int len, unsigned char *dest) {
	u8 buf[ 1024]; int i;
	u32 a, b, c, d, e, f;

	for (int i = 0; i < (int)sizeof( buf); i++)
		buf[i] = 0;
  
	memcpy( buf, src, len);
  u32 *pbuf = (u32 *)buf;

	for (i=0; i < 64; i++)
		pbuf[i+16] = ROL(1, (pbuf[i] ^ pbuf[i+8] ^ pbuf[i+2] ^ pbuf[i+13]) % 32);

	a = 0x67452301;
	b = 0xefcdab89;
	c = 0x98badcfe;
	d = 0x10325476;
	e = 0xc3d2e1f0;

  for (i ^= i; i < 20; i++) {
    f = pbuf[i] + ROL(a,5) + e + ((b & c) | (~b & d)) + (u32)0x5a827999;
    e = d;
    d = c;
    c = ROL(b,30);
    b = a;
    a = f;
  }
  
  for (; i < 40; i++) {
    f = ((d ^ c) ^ b) + e + ROL(f,5) + pbuf[i] + (u32)0x6ed9eba1;
    e = d;
    d = c;
    c = ROL(b,30);
    b = a;
    a = f;
  }
                              
  for (; i < 60; i++) {
    f = pbuf[i] + ROL(f,5) + e + ((c & b) | (d & c) | (d & b)) - (u32)0x70e44324;
    e = d;
    d = c;
    c = ROL(b,30);
    b = a;
    a = f;
  }

  for (; i < 80; i++) {
    f = (d ^ c ^ b) + e + ROL(f,5) + pbuf[i] - (u32)0x359d3e2a;
    e = d;
    d = c;
    c = ROL(b,30);
    b = a;
    a = f;
  }

  u32 *pdest = (u32 *)dest;

  pdest[0] = 0x67452301 + f;
  pdest[1] = 0xefcdab89 + b;
  pdest[2] = 0x98badcfe + c;
  pdest[3] = 0x10325476 + d;
  pdest[4] = 0xc3d2e1f0 + e;
}

void GetPasswdHash(u32 Seed, u32 SessionKey, u8 *pwHash, u8 *Out, bool DumpVars) {
	u8 Hash[28];
	u8 Result[20];

	*(u32 *)(Hash + 0) = Seed;
	*(u32 *)(Hash + 4) = SessionKey;

	memcpy(Hash + 8, pwHash, 20);

	sha1_hash(Hash, 28, Out);

	if (DumpVars == true) {
		DumpHash(Out, 20);
		DumpHash((u8*)&Seed, sizeof(u32));
		DumpHash((u8*)&SessionKey, sizeof(u32));
		DumpHash(pwHash, 20);
	}
}

void DumpHash(u8 *Hash, int Size) {
	for (int i = 0; i < Size; i++)
		printf("%.2x ", Hash[i]);

	printf("\n");
}
