#include <cctype>

#include "globals.h"
#include "consts.h"

extern CGlobals *Globals;

CGlobals::CGlobals(void) {
	// MFT
	// userdb = new CDatabase();
	// db->Load("userdb.dat");
	db = new CObj();
	db->load("db.txt");
	
	dbSettings = db->getObj("settings");
	if (!dbSettings) {
		dbSettings = new CObj("settings");
		db->append( dbSettings);
	}

	dbIPBans = db->getObj("ipbans");
	if (!dbIPBans) {
		dbIPBans = new CObj("ipbans");
		db->append( dbIPBans);
	}

	dbUsers = db->getObj("users");
	if (!dbUsers) {
		dbUsers = new CObj("users");
		db->append( dbUsers);
	}

	MessageCentre = new CMessageCentre;
	done = 0;
	Socket = INVALID_SOCKET;
	Log = new CLog("logs/globals.log", "w");
	for (int i = 0; i < MAX_CHILDREN; i++)
		ChildList[i] = NULL;

	ChildCount = 0;
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket == INVALID_SOCKET) {
		Log->Write("Error: Failed to create a socket.\n");
		exit (1);
   }

	int opttoggle = 1;
	if (setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &opttoggle, sizeof(int)) == -1) {
		Log->Write("Error: Failed to set socket options.\n");
		exit(1);
   }
}

CGlobals::~CGlobals(void) {
	db->save("db.txt");
	delete db;

	delete MessageCentre;

	for (int i = 0; i < ChildCount; i++)
		delete GetChild(i);

	close (Socket);

	// delete channels
	map<string, CChannel *>::iterator j = ChannelList.begin();
	while (j != ChannelList.end()) {
		delete j->second;
		j++;
	}
}

int CGlobals::GetChannelCount(void) {
	int count = 0;
	map <string, CChannel *>::iterator i = ChannelList.begin();
	while (i != ChannelList.end()) {
		count++;
		i++;
	}

	return (count);
}

string CGlobals::ToLower(string &str) {
	std::transform(str.begin(), str.end(), str.begin(), std::tolower);
	return (str);
}

void CGlobals::Bind(int prt) {
	sockaddr_in ServerAddress;
	bzero(&ServerAddress, sizeof(sockaddr_in));
	ServerAddress.sin_family       = AF_INET;
	ServerAddress.sin_port         = htons(prt);
	ServerAddress.sin_addr.s_addr  = INADDR_ANY;

	if (bind(Socket, (sockaddr *)&ServerAddress, sizeof(sockaddr)) == INVALID_SOCKET) {
		Log->Write("Error: Failed to bind to port %d\n", prt);
		exit (1);
   }
}

void CGlobals::Loop(void) {
	int i;

	int NewSock, sin_size = sizeof(sockaddr_in);
	sockaddr_in NewAddr;

	if (listen(Socket, LISTEN_BACKLOG) == INVALID_SOCKET) {
		Log->Write("Error: Failed to listen on socket.\n");
		exit (1);
	}

	// Pre-create our threads now, to prevent slowdowns when more users connect in the future.
	for (i = 0; i < MAX_CHILDREN; i++) {
		if (ChildList[i] == NULL) {
			ChildList[i] = new CChild(ChildCount);

			int fds[2];
			if (pipe(fds) != 0) 
				Log->Write("Failed to pipe, %s\n", strerror(errno));

			ChildFDList[i] = fds[1];
			ChildList[i]->TriggerFD = fds[0];

			ChildList[i]->Initialize();
			ChildCount++;
		}
	}

	printf("Info: War.net Server Started.\n");
	int last = 0;

	int flags = fcntl(Socket, F_GETFL, 0);
	fcntl(Socket, F_SETFL, flags | O_NONBLOCK);

	for (;!Globals->done;) {
		fd_set rfds;
		FD_ZERO( &rfds);
		FD_SET( Socket, &rfds);
		
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		int n = select( Socket + 1, &rfds, 0, 0, &tv);

		if (n < 1)
			continue;

		for (;;) {
			int NewSock = accept(Socket, (sockaddr *)&NewAddr, (socklen_t *)&sin_size);
			if (NewSock == INVALID_SOCKET) // This can fail on a signal, ignore.
				break;

			u32 IP = *(u32 *)(&NewAddr.sin_addr.s_addr);

			if (IsIP_Okay(IP) == false) {
				Log->Write("IP %s has been blocked due to banning or connection limit.\n", inet_ntoa(NewAddr.sin_addr));
				close (NewSock);
				continue;
			}

			AddConnection(IP);         
			Log->Write("Info: Got a new connection from %s:%d\n", inet_ntoa(NewAddr.sin_addr), ntohs(NewAddr.sin_port));

			// Search for a free child.
			CClient *client = new CClient(NewSock, IP, inet_ntoa(NewAddr.sin_addr), ntohs(NewAddr.sin_port));
			for (int i = last++; i < last + MAX_CHILDREN; i++)
				if (ChildList[ i % MAX_CHILDREN]->IsClient_Okay(client)) {
					client = NULL;
					break;
				}

			last++;

			if (client) {
				static time_t lastmsg = 0;
				if ((time(0) - lastmsg) >= 300) {
					Log->Write("Error: We ran out of available file descriptors!\n");
					lastmsg = time(0);
				}
				delete client;
			}
		}
	}

	printf("Info: War.net Server Is Exiting.\n");
}

unsigned CGlobals::GetClientCount(void) {
	unsigned TotalCount = 0;
	for (int i = 0; i < MAX_CHILDREN; i++) {
		if (ChildList[i] == NULL) break;

		TotalCount += ChildList[i]->GetClientCount();
   }
   
	return (TotalCount);  
}

void CGlobals::BanIP(u32 IP) {
	IPBanList[IP] = true;
}

void CGlobals::UnbanIP(u32 IP) {
	IPBanList[IP] = false;
}

bool CGlobals::IsIP_Okay(u32 IP) {
	if ((IPBanList[IP] == true) || (IPCountList[IP] >= MAX_CONPERIP))
		return (false);
	else
		return (true);

	return (true);
}

void CGlobals::AddConnection(u32 IP) {
	IPCountList[IP]++;
}

// Fixme: threadlocks!
void CGlobals::FreeConnection(u32 IP) {
	IPCountList[IP]--;
}

void CGlobals::DeleteChannel(string Name) {
	if (ChannelList[Name])
		delete ChannelList[Name];

	ChannelList.erase(Name);
}

void CGlobals::InitPubChannels(void) {
	printf("Public Channels Loaded: \n");

	for (int i = 0; PublicChannels[i]; i++) {
		CChannel *Chan = new CChannel(PublicChannels[i], CHANFLAGS_PUBLIC | CHANFLAGS_SYSTEM);
		MakeChannel(PublicChannels[i], Chan);

		CChannel *c = GetChannel(PublicChannels[i]);
		if (c)
			printf("\tChannel %s with flags [ %s ]\n", c->GetName().c_str(), c->GetFlagsStr().c_str());
	}

	printf("\n");
}
