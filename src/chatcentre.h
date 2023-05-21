#ifndef CHATCENTRE_H
#define CHATCENTRE_H

#include "types.h"
#include "client.h"
#include "clog.h"

class CChatCentre {
	CLog *Log;
public:
	CChatCentre(void);
	~CChatCentre(void);

	int Dispatch(u8 *Message, u32 Size, CClient *Client);
   void GreetChat(CClient *Client);
};

#endif
