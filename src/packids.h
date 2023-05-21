#ifndef PACKIDS_H
#define PACKIDS_H

const int MAGIC = 0xff;	// starts off every game protocol packet

enum EChatPackets {
	EID_SHOWUSER           = 1001,
	EID_JOIN               = 1002,
	EID_LEAVE              = 1003,
	EID_WHISPER            = 1004,
	EID_TALK               = 1005,
	EID_BROADCAST          = 1006,
	EID_CHANNEL            = 1007,
	EID_USERFLAGS          = 1009,
	EID_WHISPERSENT        = 1010,
	EID_CHANNELFULL        = 1013,
	EID_CHANNELDOESNOTEXIST= 1014,
	EID_CHANNELRESTRICTED  = 1015,
	EID_INFO               = 1018,
	EID_ERROR              = 1019,
	EID_EMOTE              = 1023,
	EID_UNIQUENAME         = 2010
};

// Complete list of binary packets, phew 		- moon

enum EGamePackets {
	SID_AUTH_ACCOUNTCHANGE				= 0x55,
	SID_AUTH_ACCOUNTCHANGEPROOF		= 0x56,
	SID_AUTH_ACCOUNTCREATE				= 0x52,
	SID_AUTH_ACCOUNTLOGON				= 0x53,
	SID_AUTH_ACCOUNTLOGONPROOF			= 0x54,
	SID_AUTH_ACCOUNTUPGRADE				= 0x57,
	SID_AUTH_ACCOUNTUPGRADEPROOF		= 0x58,
	SID_AUTH_CHECK							= 0x51,
	SID_AUTH_DISCONNECT					= 0x5b,
	SID_AUTH_INFO							= 0x50,
	SID_AUTH_RECONNECT					= 0x59,
	SID_AUTH_RECONNECTPROOF				= 0x5a,
	SID_CDKEY								= 0x30,
	SID_CDKEY2								= 0x36,
	SID_CHANGEPASSWORD					= 0x31,
	SID_CHATCOMMAND						= 0x0e,
	SID_CHATEVENT							= 0x0f,
	SID_CHECKAD								= 0x15,
	SID_CLICKAD								= 0x16,
	SID_CLIENTID							= 0x05,
	SID_CLIENTID2							= 0x1e,
	SID_CREATEACCOUNT						= 0x2a,
	SID_CREATEACCOUNT2					= 0x3d,
	SID_DISPLAYAD							= 0x21,
	SID_ENTERCHAT							= 0x0a,
	SID_FINDLADDERUSER					= 0x2f,
	SID_FLOODDETECTED						= 0x13,
	SID_GAMERESULT							= 0x2c,
	SID_GETADVLISTEX						= 0x09,
	SID_GETCHANNELLIST					= 0x0b,
	SID_GETFILETIME						= 0x33,
	SID_GETICONDATA						= 0x2d,
	SID_GETLADDERDATA						= 0x2e,
	SID_JOINCHANNEL						= 0x0c,
	SID_LEAVECHAT							= 0x10,
	SID_LOCALEINFO							= 0x12,
	SID_LOGONCHALLENGE					= 0x28,
	SID_LOGONCHALLENGEEX					= 0x1d,
	SID_LOGONRESPONSE						= 0x29,
	SID_LOGONRESPONSE2					= 0x3a,
	SID_MESSAGEBOX							= 0x19,
	SID_NOTIFYJOIN							= 0x22,
	SID_NULL									= 0x00,
	SID_PING									= 0x25,
	SID_QUERYADURL							= 0x41,
	SID_READUSERDATA						= 0x26,
	SID_REPORTVERSION						= 0x07,
	SID_SERVERLIST							= 0x04,
	SID_STARTADVEX2						= 0x1a,
	SID_STARTADVEX3						= 0x1c,
	SID_STARTVERSIONING					= 0x06,
	SID_STOPADV								= 0x02,
	SID_SYSTEMINFO							= 0x2b,
	SID_UDPPINGRESPONSE					= 0x14,
	SID_WRITEUSERDATA						= 0x27,
	SID_BROADCAST							= 0x20,
	SID_CDKEY3								= 0x42,
	SID_GETCOOKIE							= 0x24,
	SID_QUERYMEM							= 0x17,
	SID_SETCOOKIE							= 0x23

};

enum EFTPPackets {
	FID_FILEREQUEST						= 0x100,
	FID_FILERESPONSE						= 0x000
};

const u32 
			CEVENT_SHOWUSER			= 0x00000001, // shows current in-channel users
			CEVENT_USERJOIN			= 0x00000002, // sent to all users as an event when someone joins the chan
			CEVENT_USERPART			= 0x00000003, // user leaves chan
			CEVENT_USERWHISPERS		= 0x00000004, // user whispers
			CEVENT_USERTALKS			= 0x00000005, // user has talked
			CEVENT_BROADCAST			= 0x00000006, // server broadcast msg
			CEVENT_JOINING				= 0x00000007, // sent in response to a client joining a chan
			CEVENT_USERFLAGS			= 0x00000009, // Sent when a user's flags change
			CEVENT_WHISPERSENT		= 0x0000000a, // Sent in response to sending a whisper
			CEVENT_INFO					= 0x00000012, // Sent in response to info related commands (eg. whois bob, "bob is in channel bob")
			CEVENT_ERROR				= 0x00000013, // Error in a command, we let the user know there was an error through this msg
			CEVENT_USEREMOTES			= 0x00000017; // User emotes
#endif
