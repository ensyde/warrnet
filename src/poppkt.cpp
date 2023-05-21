#include "poppkt.h"

CPopPkt::CPopPkt( u32 pkt_len, char *pkt_buf) {
	CPopPkt::pkt_len = pkt_len;
	CPopPkt::pkt_buf = pkt_buf;
	pkt_loc = 4;
	iserror = 0;
}

CPopPkt::~CPopPkt( void) {
}

u32 CPopPkt::pop_u32( void) {
	u32 ret = 0;
	if (pkt_loc + 4 <= pkt_len) {
		ret = *(u32 *)(pkt_buf + pkt_loc);
		pkt_loc += 4;
	} else
		iserror++;

	return ret;
}

u16 CPopPkt::pop_u16( void) {
	u16 ret = 0;
	if (pkt_loc + 2 <= pkt_len) {
		ret = *(u16 *)(pkt_buf + pkt_loc);
		pkt_loc += 2;
	} else
		iserror++;

	return ret;
}

std::string CPopPkt::pop_string( void) {
	std::string ret;
	for (int i = 0; ((i + pkt_loc) < pkt_len); i++) {
		if (!*(pkt_buf + pkt_loc + i))
			break;
		ret.append( 1, *(pkt_buf + pkt_loc + i));
	}

	pkt_loc += ret.length() + 1;

	return ret;
}
