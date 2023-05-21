#ifndef TYPES_H
#define TYPES_H

typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned long BnetHash[5];

enum EConnectionType {
	C_UNKNOWN		= 0x00,
	C_GAME			= 0x01,
	C_FTP				= 0x02,
	C_CHAT			= 0x03
};

enum EChatStages {
	S_PRELOGIN		= 0,
	S_USERNAME		= 1,
	S_PASSWORD		= 2,
	S_LOGGEDIN		= 3
};

#ifndef INVALID_SOCKET
  #define INVALID_SOCKET -1
#endif

#endif
