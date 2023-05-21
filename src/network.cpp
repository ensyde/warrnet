#include "network.h"
#include "client.h"
#include "globals.h"
#include "child.h"
#include "clog.h"

extern CGlobals *Globals;
CLog Log("logs/sep.log", "s");

CNetwork::CNetwork(void) {
	StagePos = 0;
	StageBuf[0] = 0;
	Socket = INVALID_SOCKET;
}

CNetwork::~CNetwork(void) {

}

void CNetwork::OnRead(void) {
	debug(("CNetwork::OnRead"));

	// MFT, should not need to be here
	if (Socket == INVALID_SOCKET)
		return;


	static bool FirstPacket = true;
	int n = recv(Socket, StageBuf + StagePos, MAX_STAGEBUF - StagePos, 0);
	if (!n) {
		Disconnect();
		return;
	}

	if (n == -1) {
		switch (errno) {
			case EINTR: {
				return;
			} break;

			default: {
				Disconnect();
				return;
			} break;
		}
	}

	// I was hoping to use dynamic_cast<>(), but it wouldn't work here.
	// Cast this (CNetwork) to a CClient object, since the only situation
	// when this would be called would be from a CClient * object.
	CClient *Client = static_cast<CClient *>(this);

	StagePos += n;
	if (StagePos > sizeof(StageBuf))
		StagePos = 0;

	// Log.HexDump(StagePos, StageBuf);

	if (Client->FirstPacket == true) {
		Client->FirstPacket = false;
		switch ((int)StageBuf[0]) {
			// Game protocol
			case 1: {
				Globals->MsgCentre()->GetGameCentre()->GreetGame(
				"Welcome to War.net, the \"Game\" protocol is not quite finished.  Check back shortly.\n - Moonshine", Client);
				Client->SetConnectionType(C_GAME);
			} break;

			// File protocol
			case 2: {
				Client->SetConnectionType(C_FTP);
			} break;
					
			// Chat protocol
			case 3:
			case 'c': {
				Client->SetConnectionType(C_CHAT);
				Globals->MsgCentre()->GetChatCentre()->GreetChat(Client);
			} break;
		}
		StagePos--;
		memmove(StageBuf, StageBuf + 1, StagePos);
		if (!StagePos)
			return;
	}

	switch (Client->GetConnectionType()) {
		case C_UNKNOWN: {
			char *msg = "You're an unknown client, so we don't like you. @8-P~\n";
			send(Socket, msg, strlen(msg), 0);
			Disconnect();
			return;
		} break;

		case C_GAME: {
			while (StagePos) {
				if (StageBuf[0] != 0xff) {
					Disconnect();
					return;
				}
				
				if (StagePos <= 4)
					return;

				u16 Size = *(u16 *)(StageBuf + 2);
				if (Size > StagePos)
					return;
		
				Globals->MsgCentre()->Dispatch(StageBuf, Size, Client);
				StagePos -= Size;
				memmove(StageBuf, StageBuf + Size, StagePos);
			}					
		} break;

		case C_FTP: {
			if (StagePos <= 4)
				return;
		
			u16 Size = *(u16 *)(StageBuf + 0);
			if (Size > StagePos)
				return;

			Globals->MsgCentre()->Dispatch(StageBuf, Size, Client);
			StagePos -= Size;
			memmove(StageBuf, StageBuf + Size, StagePos);
		} break;

		case C_CHAT: {
			while (char *p = strchr((char*)StageBuf, '\n')) {
				*p=0;
				u32 len = strlen(p)+1;

				if (len <= 2) {
					memmove(StageBuf, StageBuf + len, StagePos - len);
					break;
				}

				if (*(p-1) != '\r') {
					Disconnect();
					return;
				}
			
				*(p-1) = 0;
				Globals->MsgCentre()->Dispatch(StageBuf, len - 2, Client);
				memmove(StageBuf, StageBuf + len, StagePos - len);
			}
		} break;

		default:break;
	}

	debug(("CNetwork::~OnRead"));
}

void CNetwork::Disconnect(void) {
	for (int i = 0; i < 8; i++)
		if (close(Socket) == 0)
			break;

	Socket = INVALID_SOCKET;
}
