#ifndef GAMECENTRE_H
#define GAMECENTRE_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <time.h>
#include <algorithm>

#include "types.h"
#include "client.h"
#include "clog.h"
#include "packids.h"
#include "poppkt.h"

using namespace std;

class CGameCentre {
	CLog *Log;

	u8 Buffer[MAX_STAGEBUF];
	u32 BufferPos;

	bool CmdCmp(const char *Str1, const char *Str2);
	void TrimCmd(char *Message);
private:
	int IsNameValid(char *Username);
	int IsCmdValid(char *Cmd);
	//
	// Chat events
	//
	
	void ParseChatCommand(char *Message, u32 Size, CClient *Client);
	void DispatchChatCommand(char *Message, CClient *Client);
	
	void OnChatCommandWho(char *Message, CClient *Client);
	void OnChatCommandWhisper(char *Message, CClient *Client);	
	void OnChatCommandEmote(char *Message, CClient *Client);
	void OnChatCommandWhois(char *Message, CClient *Client);
	void OnChatCommandHelp(char *Message, CClient *Client);   
	void OnChatCommandRejoin(char *Message, CClient *Client);
	void OnChatCommandUnsquelch(char *Message, CClient *Client);
	void OnChatCommandSquelch(char *Message, CClient *Client);
	void OnChatCommandJoin(char *Message, CClient *Client);
	void OnChatCommandAway(char *Message, CClient *Client);
	void OnChatCommandDnd(char *Message, CClient *Client);
	void OnChatCommandOptions(char *Message, CClient *Client);
	void OnChatCommandBan(char *Message, CClient *Client);
	void OnChatCommandKick(char *Message, CClient *Client);
	void OnChatCommandBeep(char *Message, CClient *Client);
	void OnChatCommandDesignate(char *Message, CClient *Client);
	void OnChatCommandNobeep(char *Message, CClient *Client);
	void OnChatCommandTime(char *Message, CClient *Client);
	void OnChatCommandUnban(char *Message, CClient *Client);
	void OnChatCommandUsers(char *Message, CClient *Client);
	void OnChatCommandWhoami(char *Message, CClient *Client);

	//
	//	Game packets
	//	
	void OnAuthAccountChange(u8 *Message, u32 Size, CClient *Client);
	void OnAuthAccountChangeProof(u8 *Message, u32 Size, CClient *Client);
	void OnAuthAccountCreate(u8 *Message, u32 Size, CClient *Client);
	void OnAuthAccountLogon(u8 *Message, u32 Size, CClient *Client);
	void OnAuthAccountLogonProof(u8 *Message, u32 Size, CClient *Client);
	void OnAuthAccountUpgrade(u8 *Message, u32 Size, CClient *Client); 
	void OnAuthAccountUpgradeProof(u8 *Message, u32 Size, CClient *Client);
	void OnAuthCheck(u8 *Message, u32 Size, CClient *Client);
	void OnAuthDisconnect(u8 *Message, u32 Size, CClient *Client);
	void OnAuthInfo(u8 *Message, u32 Size, CClient *Client);
	void OnAuthReconnect(u8 *Message, u32 Size, CClient *Client);
	void OnAuthReconnectProof(u8 *Message, u32 Size, CClient *Client);
	void OnCdkey(u8 *Message, u32 Size, CClient *Client);
	void OnCdkey2(u8 *Message, u32 Size, CClient *Client);
	void OnChangePassword(u8 *Message, u32 Size, CClient *Client);
	void OnChatCommand(u8 *Message, u32 Size, CClient *Client);
	void OnChatEvent(u8 *Message, u32 Size, CClient *Client);
	void OnCheckAd(u8 *Message, u32 Size, CClient *Client);
	void OnClickAd(u8 *Message, u32 Size, CClient *Client);
	void OnClientId(u8 *Message, u32 Size, CClient *Client);
	void OnClientId2(u8 *Message, u32 Size, CClient *Client);
	void OnCreateAccount(u8 *Message, u32 Size, CClient *Client);
	void OnCreateAccount2(u8 *Message, u32 Size, CClient *Client);
	void OnDisplayAd(u8 *Message, u32 Size, CClient *Client);
	void OnEnterChat(u8 *Message, u32 Size, CClient *Client);
	void OnFindLadderUser(u8 *Message, u32 Size, CClient *Client);
	void OnFloodDetected(u8 *Message, u32 Size, CClient *Client);
	void OnGameResult(u8 *Message, u32 Size, CClient *Client);
	void OnGetAdvListEx(u8 *Message, u32 Size, CClient *Client);
	void OnGetChannelList(u8 *Message, u32 Size, CClient *Client);
	void OnGetFileTime(u8 *Message, u32 Size, CClient *Client);
	void OnGetIconData(u8 *Message, u32 Size, CClient *Client);
	void OnGetLadderData(u8 *Message, u32 Size, CClient *Client);
	void OnJoinChannel(u8 *Message, u32 Size, CClient *Client);
	void OnLeaveChat(u8 *Message, u32 Size, CClient *Client); 
	void OnLocaleInfo(u8 *Message, u32 Size, CClient *Client);
	void OnLogonChallenge(u8 *Message, u32 Size, CClient *Client);
	void OnLogonChallengeEx(u8 *Message, u32 Size, CClient *Client);
	void OnLogonResponse(u8 *Message, u32 Size, CClient *Client);
	void OnLogonResponse2(u8 *Message, u32 Size, CClient *Client);
	void OnMessageBox(u8 *Message, u32 Size, CClient *Client);
	void OnNotifyJoin(u8 *Message, u32 Size, CClient *Client);
	void OnNull(u8 *Message, u32 Size, CClient *Client);
	void OnPing(u8 *Message, u32 Size, CClient *Client); 
	void OnQueryAdUrl(u8 *Message, u32 Size, CClient *Client);
	void OnReadUserData(u8 *Message, u32 Size, CClient *Client);
	void OnReportVersion(u8 *Message, u32 Size, CClient *Client);
	void OnServerList(u8 *Message, u32 Size, CClient *Client);
	void OnStartAdvEx2(u8 *Message, u32 Size, CClient *Client);
	void OnStartAdvEx3(u8 *Message, u32 Size, CClient *Client);
	void OnStartVersioning(u8 *Message, u32 Size, CClient *Client);
	void OnStopAdv(u8 *Message, u32 Size, CClient *Client);
	void OnSystemInfo(u8 *Message, u32 Size, CClient *Client);
	void OnUdpPingResponse(u8 *Message, u32 Size, CClient *Client);
	void OnWriteUserData(u8 *Message, u32 Size, CClient *Client);
	void OnUnknown(u8 *Message, u32 Size, CClient *Client);
   void OnBroadcast(u8 *Message, u32 Size, CClient *Client);
   void OnCdkey3(u8 *Message, u32 Size, CClient *Client);
   void OnGetCookie(u8 *Message, u32 Size, CClient *Client);
   void OnQueryMem(u8 *Message, u32 Size, CClient *Client);
   void OnSetCookie(u8 *Message, u32 Size, CClient *Client);

public:
	void SendEventUserTalks(char *Message, u32 Size, CClient *Client);
	void SendEventUserInChannel(char *Username, u32 Flags, u32 Latency, char *StatString, CClient *Client);
	void SendEventJoinChannel(char *ChannelName, CClient *Client);
	void SendEventError(char *ErrorMsg, CClient *Client);
	void SendEventInfo(char *Msg, CClient *Client);
	void SendEventBroadcast(char *Msg, CClient *Client);
	void SendEventUserFlags(char *Username, u32 Flags, u32 Latency, CClient *Client);

public:
	CGameCentre(void);
	~CGameCentre(void);

	int Dispatch(u8 *Message, u32 Size, CClient *Client);
	void GreetGame(char *Msg, CClient *Client);

	CGameCentre &operator<< (int Data);
	CGameCentre &operator<< (u8 Data );
	CGameCentre &operator<< (u16 Data);
	CGameCentre &operator<< (u32 Data);
	CGameCentre &operator<< (u8 *Data);
	CGameCentre &operator<< (char *Data);
	void SendPacket(int ID, CClient *Client);
};

#endif
