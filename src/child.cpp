#include "child.h"
#include "globals.h"

using namespace std;

extern CGlobals *Globals;

void SigIgnore( int Signal) {
}

void *ThreadHandler(void *Arg) {
   CChild *Child = (CChild *)Arg;
	Child->Instance();
	Child->Stop();
}

CChild::CChild(int ChildNum) {
	Lock();

	char filename[128];
	sprintf(filename, "logs/children.log");
	Log = new CLog(filename, "w");

	ChildNumber = ChildNum;
	for (int i = 0; i < MAX_CLIENTS; i++)
		ClientList[i] = NULL;

	ClientCount = 0;
}

CChild::~CChild(void) {
	delete Log;
	for (int i = 0; i < ClientCount; i++)
		delete ClientList[i];
}

void CChild::Initialize(void) {
   CThread::Start(ThreadHandler, (void *)this);
}

void CChild::Instance(void) {
	Unlock();

	signal (SIGPIPE, SIG_IGN);

	while (!Globals->done) {
		
		fd_set rfds, wfds, efds;
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);

		timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		int max = TriggerFD;

		FD_SET(TriggerFD, &rfds);

		for (int i = 0; i < GetClientCount(); i++) {
			CClient *client = ClientList[i];
			if (!client)
				continue;

			int fd = client->GetFD();
			if (fd == INVALID_SOCKET) {
				RemoveClient(i);
				continue;
			}

			FD_SET(fd, &rfds);
			if (fd > max)
				max = fd;
		}

		int n = select(max + 1, &rfds, NULL, NULL, &tv);
		if (!n)
			continue;

      if (n == -1) {
			if (errno == EINTR)
				continue; // ignore signals
			else {
	         Log->Write("Error: Select in thread #%d returned -1 (%s)\n", ChildNumber, strerror(errno));
     		   return;
			}
      }

		// We could get triggered many times between selects.
		if (FD_ISSET(TriggerFD, &rfds)) {
			// Read in the number 0x01
			// It seems if we don't read() everytime they send a Trigger(), the server segfaults... why?
			char buf[ 512];
			read(TriggerFD, buf, sizeof( buf));

			// MFT
			//char ch;
			//int n = read(TriggerFD, &ch, 1);
		}

		for (int i = 0; i < GetClientCount(); i++) {
			debug(("Child::Instance is on the next i, now i=>%d, ClientList[i]=>0x%.8x",i, ClientList[i]));


			CClient *client = ClientList[i];
			if (!client)
				continue;

			int temp_fd = client->GetFD();
			if (temp_fd != -1)
				if (FD_ISSET(temp_fd, &rfds))
					 client->OnRead();
			debug(("Child::Instance is looping to the next i, now i=>%d",i));
		}	
	}
}

void CChild::RemoveClient(unsigned Index) {
	Lock();

	for (int i = 0; i < 256; i++)
		if (close(ClientList[Index]->GetFD()) == 0)
			break;

	// Globals->FreeConnection(ClientList[Index]->GetIP());
	if (ClientList[Index]->UserData->Channel != "") {
		Globals->GetChannel(ClientList[Index]->UserData->Channel)->PopUser(ClientList[Index]);
	}

	Globals->DelUser(ClientList[Index]->UserData->Username);
	delete ClientList[Index];
	ClientList[Index] = NULL;

	if (ClientCount > 0)
		ClientCount--;

	Unlock();
}

//
// Gets a CClient* based on the position of itself in the list, NULL if the index# is bigger than the list
//
CClient *CChild::GetClient(unsigned Index) {
	return (ClientList[Index]);
}

bool CChild::IsClient_Okay(CClient *Client) {
	bool ret = false;

	Lock();

	for (int i = 0; i < MAX_CLIENTS; i++)
		if (ClientList[i] == NULL) {
			ClientList[i] = Client;
			ClientCount++;
			ret = true;
			Trigger( TriggerFD);
			break;
		}

	Unlock();

	return( ret);
}

void CChild::Trigger(int FD) {
	write(FD, "\x1", 1);
}
