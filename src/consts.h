#ifndef CONSTS_H
#define CONSTS_H

//#define DEBUG

#ifdef DEBUG
   #define debug( args) { printf("DEBUG: "); printf args; printf("\n"); fflush( stdout); }
   
const int 
			MAX_CHANLIMIT = 512, 
			MAX_CONPERIP = 512; 

#else 
	#define debug(args)

const int 
			MAX_CHANLIMIT = 40, 
			MAX_CONPERIP = 8; 

#endif

#ifndef NULL
	#define NULL 0
#endif

const int 
		LISTEN_BACKLOG  = 32,
		BNET_PORT       = 6112,
		MAX_NAMELEN     = 15,
		MAX_SERVERUSER  = 4096,
		MAX_BANNEDUSER  = 80,
		MAX_STAGEBUF	 = 3072,	// updated from bnetd (used to be 2048)
		MAX_CHILDTHREAD = 16,
		MAX_USERNAME	 = 16,	// *** INCLUDING NULL
		MIN_USERNAME	 = 2,
		MAX_CHATMESSAGE = 255,	// max chat msg length 
		MAX_CLIENTS     = 256,	// per child
		MAX_CHILDREN    = 16;

static const char *BannedWords[] = { 
	"nigger", 
	"chink", 
	"faggot", 
	"fag", 
	"n1gger", 
	"n1gg3r", 
	"n1gg3|2", 
	"n1gge|2" 
	"nigge|2", 
	"niggr", 
	"ch1nk", 
	"[hink",
	"(hink", 
	"warnet", 
	"war-net", 
	"war.net", 
	"war_net",
	"war`net",
	"war!net",
	"war^net",
	NULL 
};

static const char *PublicChannels[] = {
	"war-net",
	"War.net Lobby",
	"Moonshine's Sex Spa",
	"Bob's Place Thingy(tm)",
	NULL
};

const int 
			ISVALIDRET_SUCCESS		= 0,
			ISVALIDRET_BANNEDWORD 	= 1,
			ISVALIDRET_ALPHANUMERIC = 2,
			ISVALIDRET_INVALIDCHARS = 3,
			ISVALIDRET_ACCTEXISTS	= 4;			


enum EChanFlags { 
	CHANFLAGS_NONE 			= 0x00000000, 
	CHANFLAGS_PUBLIC 			= 0x00000001, 
	CHANFLAGS_MODERATED 		= 0x00000002, 
	CHANFLAGS_RESTRICTED 	= 0x00000004,
	CHANFLAGS_VOIDABLE 		= 0x00000008,
	CHANFLAGS_SYSTEM        = 0x00000020,
	CHANFLAGS_OFFICIAL      = 0x00001000
};
                     

#endif
