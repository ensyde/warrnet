#include "chatcentre.h"
#include "packids.h"
#include <stdlib.h>

CChatCentre::CChatCentre(void) {
	Log = new CLog("logs/message.log", "a");
}

CChatCentre::~CChatCentre(void) {
	delete Log;
}

int CChatCentre::Dispatch(u8 *Message, u32 Size, CClient *Client) {
	if (Size > 255) {
		Message[255] = 0;
		Size = 255;
	}
	
	for(int i = 0; i < Size; ++i) {
		if (Message[i] < 32)
			return 0;
	}

	switch (Client->GetChatStage()) {
		case S_USERNAME: {
			//set Username=Message;
			//send Password:
			Client->SetChatStage(S_PASSWORD);
		} break;
		
		case S_PASSWORD: {
			//verify name / pass combo
			Client->SetChatStage(S_LOGGEDIN);
			//send bnet motd
			//JoinChannel(Default) ??
		} break;
	
		case S_LOGGEDIN: {
			if (Message[0] == '/') {
				//do /commands	
			} else {
				// Talk()
			}	
		} break;
	}
}

void CChatCentre::GreetChat(CClient *Client) {
   	//send the motd and prompt for Username:
	printf("Greeting user...\n");
   	Client->SetChatStage(S_USERNAME);
}
