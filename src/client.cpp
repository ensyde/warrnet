#include "globals.h"
#include "client.h"

using namespace std;

extern CGlobals *Globals;

CClient::CClient(int Socket, u32 IP, char *IPStr, u16 Port) {
	CNetwork::IP = IP;
	CNetwork::Socket = Socket;
	CNetwork::Port = Port;
	strcpy(CNetwork::IPStr, IPStr);

	ConnectionType = C_UNKNOWN;
	ChatStage = S_PRELOGIN;
	FirstPacket = true;

	UserData = new CUser;
}

CClient::~CClient(void) {
	if (IP)
		Globals->FreeConnection(IP);
	delete UserData;
}

void CClient::IgnoreUser(char *Username) {
	CClient *t = Globals->GetUser(Username);
	if (!t) {
		CGameCentre *gamec = Globals->MsgCentre()->GetGameCentre();
		gamec->SendEventError("That user is not logged on.", this);
		return;
	}
	else if (t->UserData->Username == UserData->Username) {
		CGameCentre *gamec = Globals->MsgCentre()->GetGameCentre();
		gamec->SendEventError("You can't squelch yourself.", this);
		return;
	}
		
	IgnoreList.push_back(Username);
}

void CClient::UnIgnoreUser(char *Username) {
	CClient *t = Globals->GetUser(Username);
	if (!t) {
		CGameCentre *gamec = Globals->MsgCentre()->GetGameCentre();
		gamec->SendEventError("That user is not logged on.", this);
		return;
	}

	list<string>::iterator i = IgnoreList.begin();
	while (i != IgnoreList.end()) {
		string str = *i;
		if (!strcasecmp(str.c_str(), Username)) {
			IgnoreList.erase(i);
			return;
		}

		i++;	
	}
}

bool CClient::IsUserIgnored(char *Username) {
	list<string>::iterator i = IgnoreList.begin();
	while (i != IgnoreList.end()) {
		string str = *i;
		if (!strcasecmp(str.c_str(), Username))
			return (true);

		i++;	
	}

	return (false);
}
