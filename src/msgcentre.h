#ifndef MSGCENTRE_H
#define MSGCENTRE_H

#include "types.h"
#include "clog.h"
#include "client.h"

#include "ftpcentre.h"
#include "gamecentre.h"
#include "chatcentre.h"

class CMessageCentre {
	CLog *Log;
   
	CFTPCentre *FTPCentre;
	CGameCentre *GameCentre;
	CChatCentre *ChatCentre;
public:
	CMessageCentre(void);
	~CMessageCentre(void);
 
	int Dispatch(u8 *Message, u32 Size, CClient *Client);
   
	CChatCentre *GetChatCentre(void) { return (ChatCentre); }
	CFTPCentre  *GetFTPCentre(void) { return (FTPCentre); }
	CGameCentre *GetGameCentre(void) { return (GameCentre); }
};

#endif
