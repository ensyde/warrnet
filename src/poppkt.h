#ifndef POPPKT_H
#define POPPKT_H

#include <string>
#include "types.h"

class CPopPkt {
private:
	u32  pkt_len;
	int  pkt_loc;
	char *pkt_buf;
	int  iserror;
public:
	CPopPkt( u32 pkt_len, char *pkt_buf);
	~CPopPkt( void);

	u32         pop_u32( void);
	u16         pop_u16( void);
	std::string pop_string( void);
	int IsError(void) { return (iserror); }
};

#endif
