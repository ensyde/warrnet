#ifndef NETWORK_H
#define NETWORK_H

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include "types.h"
#include "consts.h"

class CNetwork {
protected:
	u8 StageBuf[MAX_STAGEBUF];
	u32 StagePos;

	int Socket;
	u32 IP;
	u16 Port;

	char IPStr[128];
public:
	CNetwork(void);
   ~CNetwork(void);

	int GetFD(void) const { return (Socket); }
	u32 GetIP(void) const { return (IP); }
	char *GetIP_Str(void) { return (IPStr); }
	u16 GetPort(void) const { return (Port); }

	void OnRead(void);

	void Disconnect(void);
};

#endif
