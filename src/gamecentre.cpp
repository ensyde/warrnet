#include <ctype.h>

#include "gamecentre.h"
#include "globals.h"
#include "chatcmds.h"
#include "obj.h"
#include "sha1.h"
#include "consts.h"

extern CGlobals *Globals;

CGameCentre::CGameCentre(void) {
	Log = new CLog("logs/gamecentre.log", "w");
	Buffer[0] = '\0';
	BufferPos = 4;
}

CGameCentre::~CGameCentre(void) {
	delete Log;
}

//
// Poor moonshine broke his hands typing all this crap. :(
//

int CGameCentre::Dispatch(u8 *Message, u32 Size, CClient *Client) {
	debug(("CGAmeCentre::Dispatch( Message => 0x%.8x, Size => %d, PacketID => 0x%.2x)", Message, Size, Message[1]));

	int PacketID = Message[1];
	switch (PacketID) {
		case SID_AUTH_ACCOUNTCHANGE:		OnAuthAccountChange(Message, Size, Client); break;
		case SID_AUTH_ACCOUNTCHANGEPROOF: 	OnAuthAccountChangeProof(Message, Size, Client); break;
		case SID_AUTH_ACCOUNTCREATE: 		OnAuthAccountCreate(Message, Size, Client); break;
		case SID_AUTH_ACCOUNTLOGON: 		OnAuthAccountLogon(Message, Size, Client); break;
		case SID_AUTH_ACCOUNTLOGONPROOF:	OnAuthAccountLogonProof(Message, Size, Client); break;
		case SID_AUTH_ACCOUNTUPGRADE: 		OnAuthAccountUpgrade(Message, Size, Client); break;
		case SID_AUTH_ACCOUNTUPGRADEPROOF: 	OnAuthAccountUpgradeProof(Message, Size, Client); break;
		case SID_AUTH_CHECK: 			OnAuthCheck(Message, Size, Client); break;
		case SID_AUTH_DISCONNECT: 		OnAuthDisconnect(Message, Size, Client); break;
		case SID_AUTH_INFO: 			OnAuthInfo(Message, Size, Client); break;
		case SID_AUTH_RECONNECT: 		OnAuthReconnect(Message, Size, Client); break;
		case SID_AUTH_RECONNECTPROOF: 		OnAuthReconnectProof(Message, Size, Client); break;
		case SID_CDKEY: 			OnCdkey(Message, Size, Client); break;
		case SID_CDKEY2: 			OnCdkey2(Message, Size, Client); break;
		case SID_CHANGEPASSWORD: 		OnChangePassword(Message, Size, Client); break;
		case SID_CHATCOMMAND: 			OnChatCommand(Message, Size, Client); break;
		case SID_CHATEVENT: 			OnChatEvent(Message, Size, Client); break;
		case SID_CHECKAD:			OnCheckAd(Message, Size, Client); break;
		case SID_CLICKAD: 			OnClickAd(Message, Size, Client); break;
		case SID_CLIENTID: 			OnClientId(Message, Size, Client); break;
		case SID_CLIENTID2: 			OnClientId2(Message, Size, Client); break;
		case SID_CREATEACCOUNT: 		OnCreateAccount(Message, Size, Client); break;
		case SID_CREATEACCOUNT2: 		OnCreateAccount2(Message, Size, Client); break;
		case SID_DISPLAYAD: 			OnDisplayAd(Message, Size, Client); break;
		case SID_ENTERCHAT: 			OnEnterChat(Message, Size, Client); break;
		case SID_FINDLADDERUSER: 		OnFindLadderUser(Message, Size, Client); break;
		case SID_FLOODDETECTED: 		OnFloodDetected(Message, Size, Client); break;
		case SID_GAMERESULT: 			OnGameResult(Message, Size, Client); break;
		case SID_GETADVLISTEX: 			OnGetAdvListEx(Message, Size, Client); break;
		case SID_GETCHANNELLIST: 		OnGetChannelList(Message, Size, Client); break;
		case SID_GETFILETIME: 			OnGetFileTime(Message, Size, Client); break;
		case SID_GETICONDATA: 			OnGetIconData(Message, Size, Client); break;
		case SID_GETLADDERDATA: 		OnGetLadderData(Message, Size, Client); break;
		case SID_JOINCHANNEL: 			OnJoinChannel(Message, Size, Client); break;
		case SID_LEAVECHAT: 			OnLeaveChat(Message, Size, Client); break;
		case SID_LOCALEINFO: 			OnLocaleInfo(Message, Size, Client); break;
		case SID_LOGONCHALLENGE:		OnLogonChallenge(Message, Size, Client); break;
		case SID_LOGONCHALLENGEEX:		OnLogonChallengeEx(Message, Size, Client); break;
		case SID_LOGONRESPONSE: 		OnLogonResponse(Message, Size, Client); break;
		case SID_LOGONRESPONSE2:		OnLogonResponse2(Message, Size, Client); break;
		case SID_MESSAGEBOX: 			OnMessageBox(Message, Size, Client); break;
		case SID_NOTIFYJOIN: 			OnNotifyJoin(Message, Size, Client); break;
		case SID_NULL: 				OnNull(Message, Size, Client); break;
		case SID_PING: 				OnPing(Message, Size, Client); break;
		case SID_QUERYADURL: 			OnQueryAdUrl(Message, Size, Client); break;
		case SID_READUSERDATA: 			OnReadUserData(Message, Size, Client); break;
		case SID_REPORTVERSION: 		OnReportVersion(Message, Size, Client); break;
		case SID_SERVERLIST: 			OnServerList(Message, Size, Client); break;
		case SID_STARTADVEX2: 			OnStartAdvEx2(Message, Size, Client); break;
		case SID_STARTADVEX3: 			OnStartAdvEx3(Message, Size, Client); break;
		case SID_STARTVERSIONING:		OnStartVersioning(Message, Size, Client); break;
		case SID_STOPADV: 			OnStopAdv(Message, Size, Client); break;
		case SID_SYSTEMINFO: 			OnSystemInfo(Message, Size, Client); break;
		case SID_UDPPINGRESPONSE:		OnUdpPingResponse(Message, Size, Client); break;
		case SID_WRITEUSERDATA: 		OnWriteUserData(Message, Size, Client); break;
   		case SID_BROADCAST:			OnBroadcast(Message, Size, Client); break;
   		case SID_CDKEY3:			OnCdkey3(Message, Size, Client); break;
   		case SID_GETCOOKIE:			OnGetCookie(Message, Size, Client); break;
   		case SID_QUERYMEM:			OnQueryMem(Message, Size, Client); break;
   		case SID_SETCOOKIE:			OnSetCookie(Message, Size, Client); break;
		default: 				OnUnknown(Message, Size, Client); break;
	}

	debug(("CGAmeCentre::~Dispatch()"));
	return (0);
}

int CGameCentre::IsNameValid(char *Username) {
	int size = strlen(Username);
	if (size <= MIN_USERNAME || size >= MAX_USERNAME)
		return (ISVALIDRET_ALPHANUMERIC);

	for (int i = 0; Username[i]; i++) {
		switch (Username[i]) {
			case '#':
			case '%':
			case '&':
			case '*':
			case '\'':
			case '\"':
			case ',':
			case '/':
			case '\\':
			case '>':
			case '<':
				return (ISVALIDRET_INVALIDCHARS);
			default:
				break;
		}

		if (Username[i] <= 32 || Username[i] >= 127)
			return (ISVALIDRET_INVALIDCHARS);
	}

	////////// Search name for banned words:

	char * tmp = new char[strlen(Username) + 1];
	strcpy(tmp, Username);
	for (int i = 0; tmp[i]; i++)	
		tmp[i] = tolower(tmp[i]);

	for (int i = 0; BannedWords[i]; i++)
		if (strstr(tmp, BannedWords[i])) {
			delete [] tmp;
			return (ISVALIDRET_BANNEDWORD);
		}

	delete [] tmp;

	return (ISVALIDRET_SUCCESS);
}

void CGameCentre::OnAuthAccountChange(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnAuthAccountChangeProof(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnAuthAccountCreate(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnAuthAccountLogon(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnAuthAccountLogonProof(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnAuthAccountUpgrade(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnAuthAccountUpgradeProof(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnAuthCheck(u8 *Message, u32 Size, CClient *Client) {
	*this << (u32)0x00
		<< "";

	SendPacket(SID_AUTH_CHECK, Client);
}

void CGameCentre::OnAuthDisconnect(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

// 0000  ff 50 3a 00 00 00 00 00 - 36 38 58 49 50 58 32 44   .P:.....68XIPX2D
// 0010  09 00 00 00 00 00 00 00 - 00 00 00 00 e0 01 00 00   ................
// 0020  09 04 00 00 09 04 00 00 - 55 53 41 00 55 6e 69 74   ........USA.Unit
// 0030  65 64 20 53 74 61 74 65 - 73 00                     ed States.
void CGameCentre::OnAuthInfo(u8 *Message, u32 Size, CClient *Client) {
	CPopPkt Pack(Size, (char*)Message);

	Pack.pop_u32();	// 0
	Client->UserData->ArchTag   = Pack.pop_u32();
	Client->UserData->ClientTag = Pack.pop_u32();
	Client->UserData->Version   = Pack.pop_u32();

	Pack.pop_u32();	// Timezone junk & stuff
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();

	Client->UserData->CountryTag = Pack.pop_string();
	Client->UserData->CountryStr = Pack.pop_string();

	if (Pack.IsError()) {
		Client->Disconnect();
		return;
	}

	*this << (u32)0
			<< (u32)rand()			// session key/num
			<< (u32)rand()
			<< (u32)0xa5f3d280	// some sort of 8-byte time-stamp
			<< (u32)0x01c0e863
			<< "IX86ver0.mpq"
			<< "A=4279427344 B=3837308351 C=3593158057 4 A=A^S B=B+C C=C^A A=A+B";

	SendPacket(SID_AUTH_INFO, Client);

	*this << (u32)time(NULL);
	SendPacket(SID_PING, Client);

	gettimeofday(&Client->UserData->LastPing, 0);
}

void CGameCentre::OnAuthReconnect(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnAuthReconnectProof(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnCdkey(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnCdkey2(u8 *Message, u32 Size, CClient *Client) {
	*this << (u32)1 << "";
	SendPacket(SID_CDKEY2, Client);
}

void CGameCentre::OnChangePassword(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnChatCommand(u8 *Message, u32 Size, CClient *Client) {
	ParseChatCommand((char *)Message, Size, Client);
}

void CGameCentre::OnChatEvent(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnCheckAd(u8 *Message, u32 Size, CClient *Client) {
   *this << (u32)1
         << (u32)0x7863702e
         << (u32)0
         << (u32)0
         << "logo1.pcx"
         << "http://war-net.sf.net/";

	SendPacket(SID_CHECKAD, Client);
}

void CGameCentre::OnClickAd(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnClientId(u8 *Message, u32 Size, CClient *Client) {
	CPopPkt Pack(Size, (char *)Message);
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();

	if (Pack.IsError()) {
		Client->Disconnect();
		return;
	}

	*this << (u32)0 << (u32)0 << (u32)0 << (u32)0;
	SendPacket(SID_CLIENTID, Client);
}

void CGameCentre::OnClientId2(u8 *Message, u32 Size, CClient *Client) {
	CPopPkt Pack(Size, (char *)Message);
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_string();
	Pack.pop_string();
	if (Pack.IsError()) {
		Client->Disconnect();
		return;
	}

	*this << (u32)0 << (u32)0 << (u32)0 << (u32)0;
	SendPacket(SID_CLIENTID, Client);
}

void CGameCentre::OnCreateAccount(u8 *Message, u32 Size, CClient *Client) {
	CPopPkt Pack(Size, (char *)Message);

	u32 PasswordHash[5];
	PasswordHash[0] = Pack.pop_u32();
	PasswordHash[1] = Pack.pop_u32();
	PasswordHash[2] = Pack.pop_u32();
	PasswordHash[3] = Pack.pop_u32();
	PasswordHash[4] = Pack.pop_u32();
	string Username = Pack.pop_string();

	CObj *user = Globals->dbUsers->getObj( Username.c_str());

	int ret = IsNameValid((char *)Username.c_str());

	// IsNameValid does not seem to work
	if (!user) { // && (ret == ISVALIDRET_SUCCESS)) {
		*this << (u32)1
				<< (u32)0
				<< (u32)0	
				<< (u32)0
				<< (u32)0;
		SendPacket(SID_CREATEACCOUNT, Client);

		user = new CObj( Username.c_str());
		user->setVarBin( "pwhash", (char *)PasswordHash, 20);
		Globals->dbUsers->append( user);

		printf("Sent creation success for account %s (0x2a)\n", Username.c_str());
	} else {
		*this << (u32)0
				<< (u32)0
				<< (u32)0
				<< (u32)0
				<< (u32)0;

		SendPacket(SID_CREATEACCOUNT, Client);
	}
}

///////// Results from 0x3d (Thanks to arta):
// 0: Account created 
// 2: Name contained invalid characters 
// 3: Name contained a banned word 
// 4: Account already exists
// 6: Name did not contain enough alphanumeric characters
/////////
void CGameCentre::OnCreateAccount2(u8 *Message, u32 Size, CClient *Client) {
	const u32
				RESULT_SUCCESS				= 0,
				RESULT_INVALIDCHARS		= 2,
				RESULT_BANNEDWORD			= 3,
				RESULT_ACCTEXISTS			= 4,
				RESULT_NAMELIMITERROR	= 6;


	CPopPkt Pack(Size, (char *)Message);

	u32 PasswordHash[5];
	PasswordHash[0] = Pack.pop_u32();
	PasswordHash[1] = Pack.pop_u32();
	PasswordHash[2] = Pack.pop_u32();
	PasswordHash[3] = Pack.pop_u32();
	PasswordHash[4] = Pack.pop_u32();
	string Username = Pack.pop_string();

	if (Pack.IsError()) {
		Client->Disconnect();
		return;
	}

	CObj *user = Globals->dbUsers->getObj( Username.c_str());

	int ret = IsNameValid((char *)Username.c_str());

	if (user) {
		*this << (u32)RESULT_ACCTEXISTS
				<< (u32)0
				<< (u32)0	
				<< (u32)0
				<< (u32)0;

		SendPacket(SID_CREATEACCOUNT2, Client);
	} 
	else if (ret == ISVALIDRET_INVALIDCHARS) {
		*this << (u32)RESULT_INVALIDCHARS
				<< (u32)0
				<< (u32)0	
				<< (u32)0
				<< (u32)0;

		SendPacket(SID_CREATEACCOUNT2, Client);
	}
	else if (ret == ISVALIDRET_BANNEDWORD) {
		*this << (u32)RESULT_BANNEDWORD
				<< (u32)0
				<< (u32)0	
				<< (u32)0
				<< (u32)0;

		SendPacket(SID_CREATEACCOUNT2, Client);
	}
	else if (ret == ISVALIDRET_ALPHANUMERIC) {
		*this << (u32)RESULT_NAMELIMITERROR
				<< (u32)0
				<< (u32)0	
				<< (u32)0
				<< (u32)0;

		SendPacket(SID_CREATEACCOUNT2, Client);
	}
	else if (ret == ISVALIDRET_SUCCESS) {
		*this << (u32)0
				<< (u32)0
				<< (u32)0	
				<< (u32)0
				<< (u32)0;

		SendPacket(SID_CREATEACCOUNT2, Client);

		// FIXME: We need to make this thread safe.
		user = new CObj( Username.c_str());
		user->setVarBin( "pwhash", (char *)PasswordHash, 20);
		Globals->dbUsers->append( user);

		// MFT
		//Globals->userdb->Push(key, PasswordHash, 20);

		printf("Sent creation success for account %s (0x3d)\n", Username.c_str());
	}
}

void CGameCentre::OnDisplayAd(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	//Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnEnterChat(u8 *Message, u32 Size, CClient *Client) {
	char msg[256];
	sprintf(msg, "%d 0 0 0 0 0 0 0 0 0", Client->UserData->ClientTag);
	*this << Message + 4
			<< msg
			<< Message + 4;

	SendPacket(SID_ENTERCHAT, Client);
}

void CGameCentre::OnFindLadderUser(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnFloodDetected(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnGameResult(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnGetAdvListEx(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnGetChannelList(u8 *Message, u32 Size, CClient *Client) {
	for (int i = 0; PublicChannels[i]; i++)
		*this << (char*)PublicChannels[i];

	*this << "";
	SendPacket(SID_GETCHANNELLIST, Client);
}

void CGameCentre::OnGetFileTime(u8 *Message, u32 Size, CClient *Client) {
	CPopPkt Pack(Size, (char *)Message);
	u32 FileType = Pack.pop_u32();
	u32 Junk1 = Pack.pop_u32();
	string Filename = Pack.pop_string();
	if (Pack.IsError()) {
		Client->Disconnect();
		return;
	}

	*this << (u32)FileType
			<< (u32)Junk1
         << (u32)0xa5f3d280   // some sort of 8-byte time-stamp
         << (u32)0x01c0e863
			<< (char *)Filename.c_str();

	SendPacket(SID_GETFILETIME, Client);
}

void CGameCentre::OnGetIconData(u8 *Message, u32 Size, CClient *Client) {
 	*this << (u32)0x95947691
			<< (u32)0x01c1a076
			<< "icons.bni";
	
	SendPacket(SID_GETICONDATA, Client);
}

void CGameCentre::OnGetLadderData(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnJoinChannel(u8 *Message, u32 Size, CClient *Client) {
	CPopPkt Pack(Size, (char *)Message);

	u32 Flags = Pack.pop_u32();

	string ChannelName = Pack.pop_string();

	if (Pack.IsError()) {
		Client->Disconnect();
		return;
	}
 
	if (Flags == 1)
		ChannelName = PublicChannels[0];

	if (Client->UserData->Channel != "") {
		// Pop user from the current channel, assign the client to the new channel.........
		CChannel *Chan = Globals->GetChannel(Client->UserData->Channel);
		if (!Chan) 
			Log->Write("Got null channel on user join!\n");

		Chan->PopUser(Client);
	
		// Take away ops if they have it.
		if (Client->UserData->Flags == 2) {
			Client->UserData->Flags = 0;
			CClient *t = Globals->GetUser(Client->UserData->DesignatedUser);
			if (t) {
				printf("Attempting to designate %s\n", t->UserData->Username.c_str());
				if (t->UserData->Channel == Chan->GetName()) {
					t->UserData->Flags = 2;
					printf("Designated %s\n", t->UserData->Username.c_str());
					for (int j = 0; j < Chan->GetClientCount(); j++) {
						CClient *tmp = Chan->GetClient(j);
						SendEventUserFlags((char *)t->UserData->Username.c_str(), t->UserData->Flags, t->UserData->Latency, tmp);
					}
				}
			}
		}			

		// There are no more users in the channel, if the channel is just a normal channel, delete it from the list
		// otherwise it's a special channel, do not delete!
		if (Chan->GetClientCount() == 0) {
			switch (Chan->GetFlags()) {
				case CHANFLAGS_NONE:
					Globals->DeleteChannel(Client->UserData->Channel);
					break;

				default: break;
			}
		}
	}

	// Update channel name
	Client->UserData->Channel = ChannelName;

	CChannel *Chan = Globals->GetChannel(ChannelName);

	if (!Chan) {
		printf("Making new channel %s...\n", ChannelName.c_str());
		Chan = new CChannel (ChannelName, CHANFLAGS_NONE);
		Globals->MakeChannel(ChannelName, Chan);
	}

	SendEventJoinChannel((char *)ChannelName.c_str(), Client);
	Chan->PushUser(Client);

	for (int j = 0; j < Chan->GetClientCount(); j++) {
		CClient *tmp = Chan->GetClient(j);
		SendEventUserInChannel((char*)tmp->UserData->Username.c_str(), tmp->UserData->Flags, tmp->UserData->Latency,
									  (char*)tmp->UserData->GetStatStr().c_str(), Client);
	} 

	// User is the only person in the channel, give them ops (non-clan channels only)

	if (Chan->GetClientCount() == 1 && strncasecmp(ChannelName.c_str(), "clan ", strlen("clan ")) && Chan->GetFlags() == 0) {
		Client->UserData->Flags = 2;
		SendEventUserFlags((char*)Client->UserData->Username.c_str(), Client->UserData->Flags,
								 Client->UserData->Latency, Client);
	}

	if (!strncasecmp(ChannelName.c_str(), "clan ", strlen("clan ")) && Chan->GetFlags() == 0) {
		const char *p = ChannelName.c_str() + strlen("clan ");
		const char *Name = Client->UserData->Username.c_str();

		if (!strcasecmp(p, Name)) {
			Client->UserData->Flags = 2;

			for (int j = 0; j < Chan->GetClientCount(); j++) {
				CClient *tmp = Chan->GetClient(j);
				SendEventUserFlags((char*)Client->UserData->Username.c_str(), Client->UserData->Flags, Client->UserData->Latency, tmp);
			}
		}
	} 
}

void CGameCentre::OnLeaveChat(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnLocaleInfo(u8 *Message, u32 Size, CClient *Client) {
	CPopPkt Pack(Size, (char *)Message);

	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_string();
	Pack.pop_string();
	Client->UserData->CountryTag = Pack.pop_string();
	Client->UserData->CountryStr = Pack.pop_string();

	if (Pack.IsError()) {
		Client->Disconnect();
		return;
	}
}

void CGameCentre::OnLogonChallenge(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnLogonChallengeEx(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnLogonResponse2(u8 *Message, u32 Size, CClient *Client) {
   u8 *Hash1, rHash[20], Result[20], Hash[28];
   u32 seed, skey;

	CPopPkt Pack(Size, (char *)Message);
	seed = Pack.pop_u32();
	skey = Pack.pop_u32();
	*(u32 *)(rHash + 0) = Pack.pop_u32();
	*(u32 *)(rHash + 4) = Pack.pop_u32();
	*(u32 *)(rHash + 8) = Pack.pop_u32();
	*(u32 *)(rHash + 12) = Pack.pop_u32();
	*(u32 *)(rHash + 16) = Pack.pop_u32();
	string name = Pack.pop_string();

	if (Pack.IsError()) {
		Client->Disconnect();
		return;
	}

	CObj *user = Globals->dbUsers->getObj( name.c_str());
	if (user) {
		u8 Result[20];
		std::string pwhash = user->getVarBin( "pwhash");

		GetPasswdHash(seed, skey, (u8 *)pwhash.c_str(), Result, false);
		if (memcmp( rHash, Result, 20)) {
			printf("Invalid password received on username \'%s\'.\n", name.c_str());
			*this << (u32)1;
			SendPacket(SID_LOGONRESPONSE2, Client);
			return;
		}

	}
	else {
		*this << (u32)1;
		SendPacket(SID_LOGONRESPONSE2, Client);
		return;
	}
			
   Client->UserData->Username = name;
	Globals->SetUser(Client->UserData->Username, Client);

   *this << (u32)0;
   SendPacket(SID_LOGONRESPONSE2, Client);

	printf("Info: User %s [ Using %s, %dms Latency ] has logged on successfully.\n",
		Client->UserData->Username.c_str(), Client->UserData->GetProductStr().c_str(), Client->UserData->Latency);
}

void CGameCentre::OnLogonResponse(u8 *Message, u32 Size, CClient *Client) {
   u8 *Hash1, rHash[20], Result[20], Hash[28];
   u32 seed, skey;

	CPopPkt Pack(Size, (char *)Message);
	seed = Pack.pop_u32();
	skey = Pack.pop_u32();
	*(u32 *)(rHash + 0) = Pack.pop_u32();
	*(u32 *)(rHash + 4) = Pack.pop_u32();
	*(u32 *)(rHash + 8) = Pack.pop_u32();
	*(u32 *)(rHash + 12) = Pack.pop_u32();
	*(u32 *)(rHash + 16) = Pack.pop_u32();
	string name = Pack.pop_string();

	if (Pack.IsError()) {
		Client->Disconnect();
		return;
	}

	CObj *user = Globals->dbUsers->getObj( name.c_str());
	if (user) {
		u8 Result[20];
		std::string pwhash = user->getVarBin( "pwhash");

		GetPasswdHash(seed, skey, (u8 *)pwhash.c_str(), Result, false);
		if (memcmp( rHash, Result, 20)) {
			printf("Invalid password received on username \'%s\'.\n", name.c_str());
			*this << (u32)0;
			SendPacket(SID_LOGONRESPONSE, Client);
			return;
		}

	}
	else {
		*this << (u32)0;
		SendPacket(SID_LOGONRESPONSE, Client);
		return;
	}
			
   Client->UserData->Username = name;
	Globals->SetUser(Client->UserData->Username, Client);

   *this << (u32)1;
   SendPacket(SID_LOGONRESPONSE, Client);

	printf("Info: User %s [ Using %s, %dms Latency ] has logged on successfully.\n",
		Client->UserData->Username.c_str(), Client->UserData->GetProductStr().c_str(), Client->UserData->Latency);
}

void CGameCentre::OnMessageBox(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnNotifyJoin(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnNull(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnPing(u8 *Message, u32 Size, CClient *Client) {
	struct timeval tv;
	gettimeofday(&tv, 0);
	Client->UserData->Latency = ((tv.tv_sec - Client->UserData->LastPing.tv_sec) * 1000) + 
										((tv.tv_usec - Client->UserData->LastPing.tv_usec) / 1000);
}

void CGameCentre::OnQueryAdUrl(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	//Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnReadUserData(u8 *Message, u32 Size, CClient *Client) {
	CPopPkt Pack(Size, (char *)Message);
	u32 NameCount = Pack.pop_u32();
	u32 KeyCount = Pack.pop_u32();
	u32 Junk = Pack.pop_u32();

	if (Pack.IsError()) {
		Client->Disconnect();
		return;
	}

	*this << NameCount
			<< KeyCount
			<< Junk;
	for (int i = 0; i < KeyCount; i++)
		*this << (u8)0;

	SendPacket(SID_READUSERDATA, Client);
}

void CGameCentre::OnReportVersion(u8 *Message, u32 Size, CClient *Client) {
	*this << (u32)2 << "" << "";
	SendPacket(SID_REPORTVERSION, Client);
}

void CGameCentre::OnServerList(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnStartAdvEx2(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnStartAdvEx3(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnStartVersioning(u8 *Message, u32 Size, CClient *Client) {
	CPopPkt Pack(Size, (char *)Message);
	Client->UserData->ArchTag = Pack.pop_u32();
	Client->UserData->ClientTag = Pack.pop_u32();
	Pack.pop_u32();
	Pack.pop_u32();

	if (Pack.IsError()) {
		Client->Disconnect();
		return;
	}
	
	*this << (u32)0xa5f3d280   // some sort of 8-byte time-stamp
			<< (u32)0x01c0e863
			<< "IX86ver0.mpq"
			<< "A=4279427344 B=3837308351 C=3593158057 4 A=A^S B=B+C C=C^A A=A+B";
	
	SendPacket(SID_STARTVERSIONING, Client);

	*this << (u32)time(NULL);
	SendPacket(SID_PING, Client);

	gettimeofday(&Client->UserData->LastPing, 0);
}

void CGameCentre::OnStopAdv(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnSystemInfo(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnUdpPingResponse(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnWriteUserData(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnBroadcast(u8 *Message, u32 Size, CClient *Client) {
   Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
   Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnCdkey3(u8 *Message, u32 Size, CClient *Client) {
   Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
   Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnGetCookie(u8 *Message, u32 Size, CClient *Client) {
   Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
   Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnQueryMem(u8 *Message, u32 Size, CClient *Client) {
   Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
   Log->HexDump( Size, (char *)Message);
}

void CGameCentre::OnSetCookie(u8 *Message, u32 Size, CClient *Client) {
   Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
   Log->HexDump( Size, (char *)Message);
}


void CGameCentre::OnUnknown(u8 *Message, u32 Size, CClient *Client) {
	Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
	Log->HexDump( Size, (char *)Message);
}


void CGameCentre::GreetGame(char *Msg, CClient *Client) {
}

CGameCentre &CGameCentre::operator<<(int Data) {
	*(int *)(Buffer + BufferPos) = Data;
	BufferPos += sizeof (int);
	return (*this);
}

CGameCentre &CGameCentre::operator<<(u8 Data) {
	*(u8 *)(Buffer + BufferPos) = Data;
	BufferPos += sizeof(u8);
	return (*this);
}

CGameCentre &CGameCentre::operator<<(u16 Data) {
	*(u16 *)(Buffer + BufferPos) = Data;
	BufferPos += sizeof(u16);
	return (*this);
}

CGameCentre &CGameCentre::operator<<(u32 Data) {
	*(u32 *)(Buffer + BufferPos) = Data;
	BufferPos += sizeof(u32);
	return (*this);
}

CGameCentre &CGameCentre::operator<<(u8 *Data) {
	operator<<((char *)Data);
	return (*this);
}

CGameCentre &CGameCentre::operator<<(char *Data) {
	if (strlen(Data) > (MAX_STAGEBUF - BufferPos))	
		return (*this);

	strcpy((char *)Buffer + BufferPos, Data);
	BufferPos += strlen(Data) + 1;
	return (*this);
}

void CGameCentre::SendPacket(int ID, CClient *Client) {
	debug(("CGameCentre::SendPacket"));
	if (!Client)
		return;

	Buffer[0] = MAGIC;
	Buffer[1] = ID;
	*(u16 *)(Buffer + 2) = BufferPos;

	if (Client->GetFD() == -1) {
		Log->Write("Error: Trying to send data to a closed fd\n");
		debug(("CGameCentre::~SendPacket - Error: Trying to send data to a closed fd"));
		return;
	}

	int n = send(Client->GetFD(), Buffer, BufferPos, 0);
	if (n == 0) {
		Client->Disconnect();
		debug(("CGameCentre::~SendPacket - Error: Socket is closed"));
		return;
	}

	if (n < 0) {
		Log->Write("Error: Failed to send data to %s\n", Client->GetIP_Str());
		Client->Disconnect();
		debug(("CGameCentre::~SendPacket - Error: Failed to send data"));
		return;
	}

	Buffer[0] = '\0';
	BufferPos = 4;

	debug(("CGameCentre::~SendPacket"));
}
void CGameCentre::SendEventUserInChannel(char *Username, u32 Flags, u32 Latency, char *StatString, CClient *Client) {
   *this << (u32)CEVENT_SHOWUSER
         << (u32)Flags
         << (u32)Latency
         << (u32)0
         << (u32)0
         << (u32)0
         << Username
         << StatString;

   SendPacket(SID_CHATEVENT, Client);
}

void CGameCentre::ParseChatCommand(char *Message, u32 Size, CClient *Client) {
	// MFT, strange /join problems ?
	// fprintf( stderr, "CGameCentre::ParseChatCommand( msg=>0x%.8x, Size=>%d, Client=>0x%.8x)\n", Message, Size, Client);
	// Log->HexDump( Size, Message);

	Message += 4;

	if (Message[0] == '/') {

		// Check to see if cmd is valid:

		bool IsCmd = false;

		for (int i = 0; ChatCommands[i] != NULL; i++) {
			// If command is in the command list, the user sent a valid command
			if (!strncasecmp(Message, ChatCommands[i], strlen(ChatCommands[i]))) {
				IsCmd = true;
				break;
			}
		}

		if (IsCmd == false) {
			SendEventError("That is not a valid command. Type /help or /? for more info.", Client);
			return;
		}
		
		if (IsCmd == true) {
			DispatchChatCommand(Message, Client);
			return;
		}
	}

	SendEventUserTalks(Message, Size, Client);
}

void CGameCentre::SendEventUserTalks(char *Message, u32 Size, CClient *Client) {
	if (Client->UserData->Channel == "")
		return;

	CChannel *Chan = Globals->GetChannel((char *)Client->UserData->Channel.c_str());
	for (int i = 0; i < Chan->GetClientCount(); i++) {
		CClient *t = Chan->GetClient(i);
		if (t == Client)
			continue;

		*this << (u32)CEVENT_USERTALKS
				<< Client->UserData->Flags
				<< Client->UserData->Latency
				<< (u32)0
				<< (u32)0
				<< (u32)0
				<< (char *)Client->UserData->Username.c_str()
				<< Message;

		SendPacket(SID_CHATEVENT, t);
	}
}		

void CGameCentre::SendEventJoinChannel(char *ChannelName, CClient *Client) {
	CChannel *Chan = Globals->GetChannel(ChannelName);
	if (!Chan) {
		printf("Info: Couldn't find channel %s\n", ChannelName);
		return;
	}

	*this << (u32)CEVENT_JOINING
			<< (u32)Chan->GetFlags()
			<< (u32)Client->UserData->Latency
			<< (u32)0
			<< (u32)0
			<< (u32)0
			<< (char *)Client->UserData->Username.c_str()
			<< ChannelName;

	SendPacket(SID_CHATEVENT, Client);
}				

void CGameCentre::SendEventUserFlags(char *Username, u32 Flags, u32 Latency, CClient *Client) {
	*this << (u32)CEVENT_USERFLAGS
			<< (u32)Flags
			<< (u32)Latency
			<< (u32)0
			<< (u32)0	
			<< (u32)0
			<< Username
			<< "";

	SendPacket(SID_CHATEVENT, Client);
}

void CGameCentre::SendEventError(char *ErrorMsg, CClient *Client) {
	*this << (u32)CEVENT_ERROR
			<< (u32)Client->UserData->Flags
			<< (u32)Client->UserData->Latency
			<< (u32)0
			<< (u32)0	
			<< (u32)0
			<< (char *)Client->UserData->Username.c_str()
			<< ErrorMsg;

	SendPacket(SID_CHATEVENT, Client);
}

void CGameCentre::SendEventInfo(char *Msg, CClient *Client) {
	*this << (u32)CEVENT_INFO
			<< (u32)Client->UserData->Flags
			<< (u32)Client->UserData->Latency
			<< (u32)0
			<< (u32)0	
			<< (u32)0
			<< (char *)Client->UserData->Username.c_str()
			<< Msg;

	SendPacket(SID_CHATEVENT, Client);
}

void CGameCentre::SendEventBroadcast(char *Msg, CClient *Client) {
	*this << (u32)CEVENT_BROADCAST
			<< (u32)Client->UserData->Flags
			<< (u32)Client->UserData->Latency
			<< (u32)0
			<< (u32)0	
			<< (u32)0
			<< (char *)Client->UserData->Username.c_str()
			<< Msg;

	SendPacket(SID_CHATEVENT, Client);
}

void CGameCentre::DispatchChatCommand(char *Message, CClient *Client) {
	if (CmdCmp(Message, ChatCommands[7]) ||
		 CmdCmp(Message, ChatCommands[8])) {

		OnChatCommandEmote(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[0]) ||
 		 CmdCmp(Message, ChatCommands[1]) ||
		 CmdCmp(Message, ChatCommands[2]) ||
		 CmdCmp(Message, ChatCommands[3])) {

		OnChatCommandWhisper(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[4]) ||
		 CmdCmp(Message, ChatCommands[5]) ||
		 CmdCmp(Message, ChatCommands[6])) {

		OnChatCommandWhois(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[9]) ||
		 CmdCmp(Message, ChatCommands[10])) {

		OnChatCommandHelp(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[11]) ||
		 CmdCmp(Message, ChatCommands[12])) {

		OnChatCommandRejoin(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[13]) ||
		 CmdCmp(Message, ChatCommands[14])) {

		OnChatCommandUnsquelch(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[15]) ||
		 CmdCmp(Message, ChatCommands[16])) {

		OnChatCommandSquelch(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[17]) ||
		 CmdCmp(Message, ChatCommands[18])) {

		OnChatCommandJoin(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[19]) ||
		 CmdCmp(Message, ChatCommands[20])) {

		SendEventError("That command is not implemented.", Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[21])) {
		OnChatCommandAway(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[22])) {
		OnChatCommandDnd(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[23]) ||
		 CmdCmp(Message, ChatCommands[24])) {

		OnChatCommandOptions(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[25])) {
		OnChatCommandBan(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[26])) {
		OnChatCommandKick(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[27])) {
		OnChatCommandBeep(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[28])) {
		OnChatCommandDesignate(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[29])) {
		OnChatCommandNobeep(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[30])) {
		SendEventError("That command is not implemented.", Client);
		return;
	}
	
	if (CmdCmp(Message, ChatCommands[31])) {
		OnChatCommandTime(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[32])) {
		OnChatCommandUnban(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[33])) {
		OnChatCommandUsers(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[34])) {
		OnChatCommandWhoami(Message, Client);
		return;
	}

	if (CmdCmp(Message, ChatCommands[35])) {
		OnChatCommandWho(Message, Client);
		return;
	}
	
	SendEventError("That is not a valid command. Type /help or /? for more info.", Client);
}

void CGameCentre::OnChatCommandWhisper(char *Message, CClient *Client) {
	// 			/m bob hi bob :)

	char Username[MAX_CHATMESSAGE + 1];
	char Msg[MAX_CHATMESSAGE + 1];

	int i = 0, j = 0, c = 0;
	for (i = 0; (Message[i] != ' ') && Message[i]; i++);

	if (!Message[i]) {
		SendEventError("That user is not logged on.", Client);
		return;
	}

	for (j = i + 1; (Message[j] != ' ') && Message[j]; j++)
		Username[c++] = Message[j];

	Username[c] = 0;
	if (isspace(Username[0]) || strlen(Username) < 1) {
		SendEventError("That user is not logged on.", Client);
		return;
	}		

	CClient *t = Globals->GetUser(Username);

	if (t != NULL) {
		if (strlen(Message + j) <= 1) {
			SendEventError("What do you want to say?", Client);
			return;
		}

		strcpy(Msg, Message + j + 1);
	
		if (strlen(Msg) < 1) {
			SendEventError("What do you want to say?", Client);
			return;
		}

		if (t->UserData->DndMsg != "") {
			char msg[256];
			sprintf(msg, "%s is unavailable (%s)", t->UserData->Username.c_str(), 
				t->UserData->DndMsg.c_str());

			SendEventInfo(msg, Client);
			return;
		}

		if (t->UserData->AwayMsg != "") {
			char msg[256];
			sprintf(msg, "%s is away (%s)", t->UserData->Username.c_str(),
				t->UserData->AwayMsg.c_str());
			SendEventInfo(msg, Client);
			return;
		}

		*this << (u32)CEVENT_WHISPERSENT
				<< (u32)Client->UserData->Flags
				<< (u32)Client->UserData->Latency
				<< (u32)0 
				<< (u32)0
				<< (u32)0
				<< (char *)t->UserData->Username.c_str()
				<< (char *)Msg;		

		SendPacket(SID_CHATEVENT, Client);

		*this << (u32)CEVENT_USERWHISPERS
				<< (u32)Client->UserData->Flags
				<< (u32)Client->UserData->Latency
				<< (u32)0 
				<< (u32)0
				<< (u32)0
				<< (char *)Client->UserData->Username.c_str()
				<< (char *)Msg;
	
		SendPacket(SID_CHATEVENT, t);
		return;
	}
	
	SendEventError("That user is not logged on.", Client);
}

void CGameCentre::OnChatCommandWho(char *Message, CClient *Client) {
    TrimCmd(Message);
    char BufferedMessage[128];
    CChannel *Chan;
    
    if (char *p = strchr(Message, ' '))
	Chan = Globals->GetChannel((char *)Client->UserData->Channel.c_str());
    else
	Chan = Globals->GetChannel(++p);
	
    if (!Chan) {
	SendEventError("That channel does not exist.\n(If you are trying to search for a user, use the /whois command.)", Client);	
	return;
    }
    
    sprintf(BufferedMessage, "Users in channel %s", (char *)Client->UserData->Channel.c_str());
    SendEventInfo(BufferedMessage, Client);
    for (int i = 0; i < Chan->GetClientCount(); i++) {
	CClient *User = Chan->GetClient(i);
	strcat(BufferedMessage, User->UserData->Username.c_str());
	if (i % 3 == 0)
	    strcat(BufferedMessage, ", ");
	else if (i % 3 == 2 || i == Chan->GetClientCount()) {
	    SendEventInfo(BufferedMessage, Client);
	    bzero(BufferedMessage, strlen(BufferedMessage));
	}
    }	    
}

void CGameCentre::OnChatCommandEmote(char *Message, CClient *Client) {
	TrimCmd(Message);

   if (Client->UserData->Channel == "")
      return;

   CChannel *Chan = Globals->GetChannel((char *)Client->UserData->Channel.c_str());
   for (int i = 0; i < Chan->GetClientCount(); i++) {
      CClient *t = Chan->GetClient(i);
      *this << (u32)CEVENT_USEREMOTES
            << Client->UserData->Flags
            << Client->UserData->Latency
            << (u32)0
            << (u32)0
            << (u32)0
            << (char *)Client->UserData->Username.c_str()
            << Message;

      SendPacket(SID_CHATEVENT, t);
   }
}

void CGameCentre::OnChatCommandWhois(char *Message, CClient *Client) {
	char msg[256];

	TrimCmd(Message);

	CClient *t = Globals->GetUser(Message);

	if (!t) {
		SendEventError("That user is not logged on.", Client);
		return;
	}

	if (t->UserData->Channel == "") {
		sprintf(msg, "%s is in a private game.", t->UserData->Username.c_str());
		SendEventInfo(msg, Client);
		return;
	}
	else {
		CChannel *Chan = Globals->GetChannel(t->UserData->Channel);
		if (!Chan) 
			return;

		u32 Flags = Chan->GetFlags();
		if (Flags & CHANFLAGS_PUBLIC) {
			sprintf(msg, "%s is using %s in the channel %s.", 
					  t->UserData->Username.c_str(), t->UserData->GetProductStr().c_str(), t->UserData->Channel.c_str()); 
			SendEventInfo(msg, Client);
		}
		else {
			sprintf(msg, "%s is using %s in a private channel.",
					  t->UserData->Username.c_str(), t->UserData->GetProductStr().c_str());

			SendEventInfo(msg, Client);
		}
	}
}
	
void CGameCentre::OnChatCommandHelp(char *Message, CClient *Client) {
	char *p = strchr(Message, ' ');
	if (!p) {
		SendEventInfo("War.net Help menu:", Client);
		SendEventInfo("Type \'/help listcmds\' to display all currently supported commands.", Client);
		SendEventInfo("Type \'/help [command]\' to display information about a command.", Client);
		SendEventInfo("Type \'/help intro\' to show a Quick-start introduction to War.net.", Client);
		return;
	}

	*p++;	// skip past ' '

	if (!strcasecmp(p, "intro")) {
		SendEventError("Help introduction incomplete.", Client);
		return;
	}

	if (!strcasecmp(p, "listcmds")) {
		SendEventInfo("War.net currently supported cmds:", Client);
		for (int i = 0; ChatCommands[i]; i++)
			SendEventInfo((char *)ChatCommands[i], Client);

		return;
	}

	if (!IsCmdValid(p)) {
		SendEventError("Not a valid help section command.", Client);
		return;
	}

	for (int i = 0; ChatCommands[i]; i++) {
		if (CmdCmp(p, ChatCommands[i])) {
			SendEventInfo((char*)ChatCmdHelp[i], Client);
			return;
		}
	}

	SendEventError("Not a valid help section command.", Client);
}
		
void CGameCentre::OnChatCommandDesignate(char *Message, CClient *Client) {
	TrimCmd(Message);

	CClient *t = Globals->GetUser(Message);
	if (!t) {
		SendEventError("That user is not logged on.", Client);
		return;
	}
	else {
		if (t->UserData->Channel != Client->UserData->Channel) {
			SendEventError("Invalid user.", Client);
			return;
		}
	}

	Client->UserData->DesignatedUser = Message;	
}

void CGameCentre::OnChatCommandJoin(char *Message, CClient *Client) {
	TrimCmd(Message);
	string ChannelName = Message;
	
	if (Client->UserData->Channel != "") {
		// Pop user from the current channel, assign the client to the new channel.........
		CChannel *Chan = Globals->GetChannel(Client->UserData->Channel);
		if (!Chan) 
			Log->Write("Got null channel on user join!\n");

		Chan->PopUser(Client);
	
		// Take away ops if they have it.
		if (Client->UserData->Flags == 2) {
			Client->UserData->Flags = 0;
			CClient *t = Globals->GetUser(Client->UserData->DesignatedUser);
			if (t) {
				printf("Attempting to designate %s\n", t->UserData->Username.c_str());
				if (t->UserData->Channel == Chan->GetName()) {
					t->UserData->Flags = 2;
					printf("Designated %s\n", t->UserData->Username.c_str());
					for (int j = 0; j < Chan->GetClientCount(); j++) {
						CClient *tmp = Chan->GetClient(j);
						SendEventUserFlags((char *)t->UserData->Username.c_str(), t->UserData->Flags, t->UserData->Latency, tmp);
					}
				}
			}
		}			

		// There are no more users in the channel, if the channel is just a normal channel, delete it from the list
		// otherwise it's a special channel, do not delete!
		if (Chan->GetClientCount() == 0) {
			switch (Chan->GetFlags()) {
				case CHANFLAGS_NONE:
					Globals->DeleteChannel(Client->UserData->Channel);
					break;

				default: break;
			}
		}
	}

	// Update channel name
	Client->UserData->Channel = ChannelName;

	CChannel *Chan = Globals->GetChannel(ChannelName);

	if (!Chan) {
		printf("Making new channel %s...\n", ChannelName.c_str());
		Chan = new CChannel (ChannelName, CHANFLAGS_NONE);
		Globals->MakeChannel(ChannelName, Chan);
	}

	SendEventJoinChannel((char *)ChannelName.c_str(), Client);
	Chan->PushUser(Client);

	for (int j = 0; j < Chan->GetClientCount(); j++) {
		CClient *tmp = Chan->GetClient(j);
		SendEventUserInChannel((char*)tmp->UserData->Username.c_str(), tmp->UserData->Flags, tmp->UserData->Latency,
									  (char*)tmp->UserData->GetStatStr().c_str(), Client);
	} 

	// User is the only person in the channel, give them ops (non-clan channels only)

	if (Chan->GetClientCount() == 1 && strncasecmp(ChannelName.c_str(), "clan ", strlen("clan ")) && Chan->GetFlags() == 0) {
		Client->UserData->Flags = 2;
		SendEventUserFlags((char*)Client->UserData->Username.c_str(), Client->UserData->Flags,
								 Client->UserData->Latency, Client);
	}

	if (!strncasecmp(ChannelName.c_str(), "clan ", strlen("clan ")) && Chan->GetFlags() == 0) {
		const char *p = ChannelName.c_str() + strlen("clan ");
		const char *Name = Client->UserData->Username.c_str();

		if (!strcasecmp(p, Name)) {
			Client->UserData->Flags = 2;

			for (int j = 0; j < Chan->GetClientCount(); j++) {
				CClient *tmp = Chan->GetClient(j);
				SendEventUserFlags((char*)Client->UserData->Username.c_str(), Client->UserData->Flags, Client->UserData->Latency, tmp);
			}
		}
	} 
}

void CGameCentre::OnChatCommandBan(char *Message, CClient *Client) {
	char msg[256], User[64], Reason[256];
	TrimCmd(Message);
	CChannel *Chan = Globals->GetChannel(Client->UserData->Channel);
	
	char *p = strchr(Message, ' ');
	if (!p) {
		if (strlen(Message) > 15) {
			SendEventError("Invalid user.", Client);
			return;
		}else{
		    strcpy(User,Message);
		}
	}else{
		*p = 0;
		if (strlen(Message) > 15) {
			SendEventInfo("Invalid user.", Client);
			return;
		}else{
		    strcpy(User,Message);
		}
		
		strcpy(Reason,p+1);
	}
	
	// does he have ops!?!?!?!?
	if (!Client->UserData->Flags & 2) {
		SendEventError("You are not a channel operator.", Client);
		return;
	}
	
	CClient *UserClient = Globals->GetUser(User);
	if (!UserClient) {
		SendEventError("That user is not logged on.", Client);
		return;
	}	    
	
	if (UserClient->UserData->Flags & 2) {
		SendEventError("You can't ban a channel operator.", Client);
		return;
	}
		
	sprintf(msg, "%s was banned by %s (%s)", UserClient->UserData->Username.c_str(), Client->UserData->Username.c_str(), Reason);
	for (int j = 0; j < Chan->GetClientCount(); j++)
		SendEventInfo(msg, Chan->GetClient(j));
	
	if (UserClient->UserData->Channel == Client->UserData->Channel)
		OnChatCommandJoin("The Void", UserClient);
	
	bool Found = 0;
	for (int j = 0; j < Chan->GetBanListCount(); j++) {
		string BannedUser = Chan->GetBanList(j);
		if (strcasecmp(User,BannedUser.c_str()))
			Found = 1;
	}
	
	if (!Found)
	    Chan->PushBanList(User);
}

void CGameCentre::OnChatCommandRejoin(char *Message, CClient *Client) {
	// A quick hack
	char msg[256];
	sprintf(msg, "/join %s", Client->UserData->Channel.c_str());
	OnChatCommandJoin(msg, Client);
}

// Ignore / unignore are going to take more work than I anticipated,
// they will need to ignore people by IP, and also display the 'X' icon
// for ignored users.

void CGameCentre::OnChatCommandUnsquelch(char *Message, CClient *Client) {
/*
	TrimCmd(Message);
	if (!Client->IsUserIgnored(Message))
		return;

	Client->UnIgnoreUser(Message);
*/
}

void CGameCentre::OnChatCommandSquelch(char *Message, CClient *Client) {
/*
	TrimCmd(Message);
	if (Client->IsUserIgnored(Message))
		return;
*/
}

void CGameCentre::OnChatCommandUsers(char *Message, CClient *Client) {
	char msg[256];
	sprintf(msg, "There are currently %d users and %d channels on War.net.", 
				Globals->GetClientCount(), Globals->GetChannelCount());

	SendEventInfo(msg, Client);	
}

void CGameCentre::OnChatCommandWhoami(char *Message, CClient *Client) {
	char msg[256];
	sprintf(msg, "You are %s, using %s in channel %s.",
				Client->UserData->Username.c_str(), Client->UserData->GetProductStr().c_str(),
				Client->UserData->Channel.c_str());

	SendEventInfo(msg, Client);
}

void CGameCentre::OnChatCommandAway(char *Message, CClient *Client) {
	TrimCmd(Message);
	if (Client->UserData->AwayMsg == "") {
		SendEventInfo("You are now marked as being away.", Client);
		Client->UserData->AwayMsg = Message;
	}
	else {
		if (Message[0]) {
			SendEventInfo("You are now marked as being away.", Client);
			Client->UserData->AwayMsg = Message;
		}
		else {
			SendEventInfo("You are no longer marked as away.", Client);
			Client->UserData->AwayMsg = "";
		}
	}
}
void CGameCentre::OnChatCommandDnd(char *Message, CClient *Client) {
	TrimCmd(Message);
	if (Client->UserData->DndMsg == "") {

		SendEventInfo("Do Not Disturb mode engaged.", Client);
		Client->UserData->DndMsg = Message;
	}
	else {
		if (Message[0]) {
			SendEventInfo("Do Not Disturb mode engaged.", Client);
			Client->UserData->DndMsg = Message;	
		}
		else {
			SendEventInfo("Do Not Disturb mode cancelled.", Client);
			Client->UserData->DndMsg = "";
		}
	}
}
void CGameCentre::OnChatCommandOptions(char *Message, CClient *Client) {}
void CGameCentre::OnChatCommandKick(char *Message, CClient *Client) {
	printf("Debug: CGameCentre::SendEventUserTalks - '%s'\n", Message);

	if (!strncmp( Message, "/kick ", 6)) {
		if (Client->UserData->Flags & 2) {
			CChannel *Chan = Globals->GetChannel((char *)Client->UserData->Channel.c_str());
			std::string tmp = Message + 6;		
			printf("Debug: Kicking user %s\n", tmp.c_str());
			Chan->KickUser( tmp);
		}
		return;
	}

}

void CGameCentre::OnChatCommandBeep(char *Message, CClient *Client) {}
void CGameCentre::OnChatCommandNobeep(char *Message, CClient *Client) {}
void CGameCentre::OnChatCommandTime(char *Message, CClient *Client) {}
void CGameCentre::OnChatCommandUnban(char *Message, CClient *Client) {}

int CGameCentre::IsCmdValid(char *Cmd) {
	for (int i = 0; ChatCommands[i]; i++) {
		if (CmdCmp(ChatCommands[i], Cmd))
			return (1);
	}

	return (0);
}

bool CGameCentre::CmdCmp(const char *Str1, const char *Str2) {
	char s1[256], s2[256];

	strcpy(s1, Str1);
	strcpy(s2, Str2);

	char *ps1 = s1;
	char *ps2 = s2;

	if (ps1[0] == '/')
		*ps1++;

	if (ps2[0] == '/')
		*ps2++;

	char *p = strchr(ps1, ' ');
	if (p)
		*p = 0;
	
	p = strchr(ps2, ' ');
	if (p)
		*p = 0;

	if (!strcasecmp(ps1, ps2)) 
		return (true);

	return (false);
}

// Trims off the /cmd in a message
void CGameCentre::TrimCmd(char *Message) {
	char *p = strchr(Message, ' ');
	if (p) {
		*p++;
		strcpy(Message, p);
	}
	else
		Message[0] = '\0';
}
