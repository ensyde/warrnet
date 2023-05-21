#include "channel.h"
#include "globals.h"

extern CGlobals *Globals;

CChannel::CChannel(string cName, u32 cFlags) : Name (cName), Flags(cFlags) {
	users = new std::list<CClient *>;
	BanList = new std::list<string>;
	UserCount = 0;
	BanListCount = 0;
}

CChannel::~CChannel(void) {
	delete users;
	delete BanList;
}

void CChannel::PushBanList(string user) {
	BanList->push_back( user);
	BanListCount++;
}

void CChannel::PopBanList(string user) {
    	BanList->remove(user);
	BanListCount--;
}

string CChannel::GetBanList(int Index) {
	std::list<string>::iterator i = BanList->begin();
	for (int c = 0; c < Index && i != BanList->end(); i++, c++);
	if (i == BanList->end())
		return (NULL);

	return (*i);
}

void CChannel::PushUser( CClient *user) {
	// Inform all channel users of new user arrival:
	CGameCentre *GameCentre = Globals->MsgCentre()->GetGameCentre();
	for (int j = 0; j < GetClientCount(); j++) {
		CClient *Client = GetClient(j);
		*GameCentre << (u32)CEVENT_USERJOIN
						<< (u32)user->UserData->Flags
						<< (u32)user->UserData->Latency
						<< (u32)0
						<< (u32)0
						<< (u32)0
						<< (char *)user->UserData->Username.c_str()
						<< (char *)user->UserData->GetStatStr().c_str();

		GameCentre->SendPacket(SID_CHATEVENT, Client);
	}

	// Push user to list:
	users->push_back( user);
	UserCount++;
}

void CChannel::PopUser( CClient *user) {
	// Notify all channel users of user depart
	CGameCentre *GameCentre = Globals->MsgCentre()->GetGameCentre();
	for (int j = 0; j < GetClientCount(); j++) {
		CClient *Client = GetClient(j);
		if (Client->GetFD() == INVALID_SOCKET)
			continue;

		*GameCentre << (u32)CEVENT_USERPART
						<< (u32)user->UserData->Flags
						<< (u32)user->UserData->Latency
						<< (u32)0
						<< (u32)0
						<< (u32)0
						<< (char *)user->UserData->Username.c_str()
						<< (char *)user->UserData->GetStatStr().c_str();

		GameCentre->SendPacket(SID_CHATEVENT, Client);
	}

	users->remove(user);
	UserCount--;
}

CClient *CChannel::GetClient(int Index) {
	std::list<CClient *>::iterator i = users->begin();
	for (int c = 0; c < Index && i != users->end(); i++, c++);
	if (i == users->end())
		return (NULL);

	return (*i);
}

string CChannel::GetFlagsStr(void) {
	std::string ret = "";

	if (Flags == 0) {
		ret = "Normal Channel";
		return (ret);
	}

	if (Flags & CHANFLAGS_PUBLIC)
		ret += "Public Channel, ";

	if (Flags & CHANFLAGS_MODERATED)
		ret += "Moderated Channel, ";

	if (Flags & CHANFLAGS_RESTRICTED)
		ret += "Restricted Channel, ";

	if (Flags & CHANFLAGS_VOIDABLE)
		ret += "Void Channel, ";
	
	if (Flags & CHANFLAGS_SYSTEM)
		ret += "System Channel, ";

	if (Flags & CHANFLAGS_OFFICIAL)
		ret += "Official Channel, ";

	ret[ret.length() - 2] = 0; // trim ", "
	return (ret);
}

void CChannel::DisplayUsers(void) {
	printf("Users in channel %s:\n", GetName().c_str());
	for (int i = 0; i < GetClientCount(); i++) {
		CClient *t = GetClient(i);
		if (!t)
			continue;

		printf("%s\n", t->UserData->Username.c_str());
	}
}

void CChannel::KickUser( std::string &username) {
	printf("Debug: CChannel::KickUser was called. Following is all the users in the channel:\n");
	for (std::list<CClient *>::iterator i = users->begin(); i != users->end(); i++) {
		printf("  '%s'\n", (*i)->UserData->GetUsername().c_str());
	}

	printf("Debug: CChannel::KickUser will now try and kick the user\n");
	for (std::list<CClient *>::iterator i = users->begin(); i != users->end(); i++) {
		printf("CChannel::KickUser comparing '%s' to '%s'\n", (*i)->UserData->GetUsername().c_str(), username.c_str());
		if ((*i)->UserData->GetUsername() == username) {
			PopUser(*i);
			return;
		}
	}
}
