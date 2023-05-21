#include "msgcentre.h"

CMessageCentre::CMessageCentre(void) {
	Log = new CLog("logs/message.log", "a");

	FTPCentre = new CFTPCentre;
	ChatCentre = new CChatCentre;
	GameCentre = new CGameCentre;
}

CMessageCentre::~CMessageCentre(void) {
	delete Log;

	delete FTPCentre;
	delete ChatCentre;
	delete GameCentre;
}

int CMessageCentre::Dispatch(u8 *Message, u32 Size, CClient *Client) {
	debug(("CMessageCentre::Dispatch"));
	
	switch (Client->GetConnectionType()) {
		case C_FTP: {
			FTPCentre->Dispatch(Message, Size, Client);
		} break;

		case C_GAME: {
			GameCentre->Dispatch(Message, Size, Client);
		} break;

		case C_CHAT: {
			ChatCentre->Dispatch(Message, Size, Client);
		} break;
	}

	debug(("CMessageCentre::~Dispatch"));
	return (0);
}
