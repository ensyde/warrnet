#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <list>

#include "network.h"
#include "types.h"
#include "user.h"

using namespace std;

class CClient : public CNetwork {
	list <string> IgnoreList;

	EConnectionType ConnectionType;
	EChatStages ChatStage;
public:
	CUser *UserData;
	bool FirstPacket;
public:	
	CClient(int Socket, u32 IP, char *IPStr, u16 Port);
	~CClient(void);

	u8 *GetStageBuf(void) { return (StageBuf); }
	u32 &GetStagePos(void) { return (StagePos); }
	EConnectionType GetConnectionType(void) const { return (ConnectionType); }
	void SetConnectionType(EConnectionType type) { ConnectionType = type; }
	EChatStages const GetChatStage (void) { return ChatStage; }
	void SetChatStage (EChatStages lStage) { ChatStage = lStage; }

	void IgnoreUser(char *Username);
	void UnIgnoreUser(char *Username);
	bool IsUserIgnored(char *Username);
};

#endif
