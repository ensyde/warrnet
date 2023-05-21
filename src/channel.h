#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>
#include <list>

#include "consts.h"
#include "client.h"

class CChannel {
private:
	std::list <CClient *> *users;
	std::list <string> *BanList;
	u16 UserCount, BanListCount;
	u32 Flags;
	string Name;
public:
	CChannel(string cName, u32 cFlags);
	~CChannel(void);

	void PushUser(CClient *user);
	void PopUser(CClient *user);
	void PushBanList(string user);
	void PopBanList(string user);
	string GetBanList(int Index);
	CClient *GetClient(int Index);
	u16 GetClientCount(void) const { return (UserCount); }
	u16 GetBanListCount(void) const { return (BanListCount); }
	u32 GetFlags(void) const { return (Flags); }
	string &GetName(void) { return (Name); }
	string GetFlagsStr(void);

	void KickUser(std::string &username);
	
	void DisplayUsers(void);
};

#endif
