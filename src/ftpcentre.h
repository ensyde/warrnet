#ifndef FTPCENTRE_H
#define FTPCENTRE_H

#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
                     
#include "types.h"
#include "client.h"
#include "packids.h"
#include "clog.h"

using namespace std;

class CFTPCentre {
	CLog *Log;
   u8 Buffer[MAX_STAGEBUF];
   u32 BufferPos;
      
private:
	void OnFileRequest(u8 *Message, u32 Size, CClient *Client);
	void OnUnknown(u8 *Message, u32 Size, CClient *Client);
	void SendFile(char *Filename, u32 Offset, u32 AdID, u32 Extension, u32 StampHigh, u32 StampLow, CClient *Client);
	int Send(int FD, const char *Buffer, int Size);
	int ReadFile(const char *Filename, char *Buffer, int Size, int Offset);
public:
	CFTPCentre(void);
	~CFTPCentre(void);

	int Dispatch(u8 *Message, u32 Size, CClient *Client);

	CFTPCentre &operator<< (int Data);
	CFTPCentre &operator<< (u8 Data );
	CFTPCentre &operator<< (u16 Data);
	CFTPCentre &operator<< (u32 Data);
	CFTPCentre &operator<< (u8 *Data);
	CFTPCentre &operator<< (char *Data);
	void SendPacket(u16 ID, CClient *Client);
};

#endif
