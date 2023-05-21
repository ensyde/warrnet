#ifndef USER_H
#define USER_H

#include "types.h"
#include "consts.h"

#include <string>
#include <sys/time.h>
#include <time.h>
#include <list>

using namespace std;

class CUser {
public:
	struct timeval LastPing;
	u32 ClientTag;
	u32 ArchTag;
	u32 Version;
	u32 Latency;
	u32 Flags;
	string DesignatedUser;
	string CountryTag;
	string CountryStr;
	string Username;                  
	string Channel;	
	string AwayMsg;
	string DndMsg;
public:
	CUser( void);
	~CUser( void);
	
	string GetUsername(void);
	string GetStatStr(void);
	string GetProductStr(void);
};

#endif
