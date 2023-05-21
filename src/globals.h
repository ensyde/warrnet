#ifndef GLOBAL_H
#define GLOBAL_H

#include "types.h"
#include "consts.h"
#include "clog.h"
#include "obj.h"
#include "channel.h"
#include "child.h"
#include "threads.h"
#include "msgcentre.h"

#include <errno.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <algorithm>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <map>

using namespace std;

class CGlobals {
private:
	CLog *Log;
	int Socket;

	int ChildFDList[MAX_CHILDREN];	// Used for triggering threads	
	CChild *ChildList[MAX_CHILDREN];
	unsigned ChildCount;

	map<u32, bool> IPBanList;         // <IP, BOOL> true=banned, false=notbanned
	map<u32, u32>  IPCountList;       // <IP, IP Connection count> 8 is max

	map<string, CChannel*> ChannelList;	// TODO: Put in private section after debugging
	map<string, CClient *> UserList;
	CMessageCentre *MessageCentre;
	
	string ToLower(string &str);
public:
	// Various Databases.
	CObj *db;
	CObj *dbUsers;
	CObj *dbSettings;
	CObj *dbIPBans;

	volatile int done; // The server is quitting
public:
	CGlobals(void);
   ~CGlobals(void);
   
	void Loop(void);
	void Bind(int prt);

	unsigned GetChildCount(void) const { return (ChildCount); }
	unsigned GetClientCount(void);
	CChild *GetChild(int Index) const { return (ChildList[Index]); }

	int GetChannelCount(void);
	CChannel *GetChannel(string Name) { return (ChannelList[ToLower(Name)]); }
	void MakeChannel(string Name, CChannel *ref) { ChannelList[ToLower(Name)] = ref; }
	void DeleteChannel(string Name);
	void InitPubChannels(void);

	CClient *GetUser(string Name) { return (UserList[ToLower(Name)]); }
	void SetUser(string Name, CClient *Client) { UserList[ToLower(Name)] = Client; }
	void DelUser(string Name) { UserList.erase(Name); }

	CMessageCentre *MsgCentre(void)const { return (MessageCentre); }

	void BanIP(u32 IP);
	void UnbanIP(u32 IP);
	bool IsIP_Okay(u32 IP);
	void AddConnection(u32 IP);
	void FreeConnection(u32 IP);
};

#endif
