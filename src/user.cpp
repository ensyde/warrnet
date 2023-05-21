#include "user.h"

using namespace std;

CUser::CUser( void) {
	ClientTag = 0;
	ArchTag = 0;
	Version = 0;
	CountryTag = "";
	CountryStr = "";
	Username = "";
	Channel = "";
	Latency = 0;
	Flags = 0;
	LastPing.tv_sec = 0;
	LastPing.tv_usec = 0;
	DesignatedUser = "";
	AwayMsg = "";
	DndMsg = "";
}

CUser::~CUser( void) {
}

string CUser::GetUsername(void) {
	return Username;
}

string CUser::GetStatStr(void) {
	char tmp[5];
	*(u32 *)(tmp + 0) = ClientTag;
	tmp[4] = 0;

	string StatStr = tmp;
	StatStr += " 0 0 0 0 0 0 0 0 0";
	return (StatStr);
}

string CUser::GetProductStr(void) {
	string ret = "";

	switch (ClientTag) {

		//
		// Starcraft Series
		//
		case (u32)'STAR':
			ret = "Starcraft";
			break;
		case (u32)'SSHR':
			ret = "Starcraft Shareware";
			break;
		case (u32)'SEXP':
			ret = "Starcraft Broodwar";
			break;
		case (u32)'JSTR':
			ret = "Starcraft Japan";
			break;
		case (u32)'JSHR':
			ret = "Starcraft Japan Shareware";
			break;

		//
		// Warcraft Series
		//
		case (u32)'W2BN':
			ret = "Warcraft II";
			break;
		case (u32)'WAR3':
			ret = "Warcraft III";
			break;

		//
		// Diablo Series
		//
		case (u32)'D2DV':
			ret = "Diablo II";
			break;
		case (u32)'D2XP':
			ret = "Diablo II Lord of Destruction";
			break;
		case (u32)'DRTL':
			ret = "Diablo";
			break;
		case (u32)'DSHR':
			ret = "Diablo Shareware";
			break;

		//
		// Other
		//
		default:
			ret = "Unknown Product";
			break;
	}

	return (ret);
}
