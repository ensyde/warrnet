#include <string>
#include <stdio.h>
#include "../poppkt.h"

// 0000  ff 50 3a 00 00 00 00 00 - 36 38 58 49 50 58 32 44   .P:.....68XIPX2D
// 0010  09 00 00 00 00 00 00 00 - 00 00 00 00 e0 01 00 00   ................
// 0020  09 04 00 00 09 04 00 00 - 55 53 41 00 55 6e 69 74   ........USA.Unit
// 0030  65 64 20 53 74 61 74 65 - 73 00                     ed States.

char buf[4*16+10] = {
	0xff, 0x50, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x38, 0x58, 0x49, 0x50, 0x58, 0x32, 0x44,
	0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00,
	0x09, 0x04, 0x00, 0x00, 0x09, 0x04, 0x00, 0x00, 0x55, 0x53, 0x41, 0x00, 0x55, 0x6e, 0x69, 0x74,
	0x65, 0x64, 0x20, 0x53, 0x74, 0x61, 0x74, 0x65, 0x73, 0x00
};

int main( void) {
	CPopPkt pkt( sizeof( buf), buf);

	u32 unk1 = pkt.pop_u32();
	u32 proc = pkt.pop_u32();
	u32 game = pkt.pop_u32();
	u32 ver = pkt.pop_u32();
	u32 unk2 = pkt.pop_u32();
	u32 unk3 = pkt.pop_u32();
	u32 bias = pkt.pop_u32();
	u32 local1 = pkt.pop_u32();
	u32 local2 = pkt.pop_u32();
	std::string ctitle = pkt.pop_string();
	std::string cname = pkt.pop_string();
	
	printf("CTitle: '%s'\n", ctitle.c_str());
	printf("CName:  '%s'\n", cname.c_str());

	return 0;
}
