#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>

#include "threads.h"
#include "child.h"
#include "client.h"
#include "globals.h"
#include "clog.h"

CGlobals *Globals;

void Daemonize(void);
void InitDirectories(void);
void SigHandler(int Signal);
int  Help(void);


int main(int argc, char **argv) {
	if (signal(SIGINT, SigHandler) == SIG_IGN) signal (SIGINT, SIG_IGN);
	if (signal(SIGTERM, SigHandler) == SIG_IGN) signal (SIGTERM, SIG_IGN);
	if (signal(SIGPIPE, SigHandler) == SIG_IGN) signal (SIGPIPE, SIG_IGN);

	int myport = 6112;

	argv++;
	while (argc > 1) {
		if (!strcmp( *argv, "-d") || !strcmp( *argv, "--daemon")) Daemonize();
		if (!strcmp( *argv, "-v") || !strcmp( *argv, "--verbose")) {
			#undef debug(args)
			#define debug( args) { printf("DEBUG: "); printf args; printf("\n"); fflush( stdout); }
		}
		if (!strcmp( *argv, "-p") || !strcmp( *argv, "--port")) {
			if (argc > 1) {
				argv++;
				argc--;
				
				myport = atoi( *argv);
			}
		}
		if (!strcmp( *argv, "-h") || !strcmp( *argv, "--help"))   return Help();
		argv++;
		argc--;
	}

	Globals = new CGlobals();

	InitDirectories();
	Globals->Bind( myport);
	Globals->InitPubChannels();
	Globals->Loop();  

	delete Globals;

	return (0);
}

void Daemonize(void) {
	CLog *Log = new CLog ("logs/daemonize.log", "w");

	switch (fork()) {
		case -1: {
			Log->Write("Error: Daemonization failed to fork. (%s)\n", strerror(errno));
 			exit (1);
		} break;

		case 0: {
		} break;

		default: {
			exit (1);
		} break;
	}
		
	if (setsid() < 0) {
		Log->Write("Error: Daemonization failed to set SID. (%s)\n", strerror(errno));
		exit (1);
	}


	for (int i = 0; i < 3; i++) {
		if (close (i) != 0) {
			Log->Write("Error: Daemonization failed to close standard fd #%d (%s)\n", i, strerror(errno));
			exit (1);
		}
	}

/* TODO: Where can we put this so it works? =\
	if (chdir("/")) { 
		Log->Write("Error: Daemonization could not set working directory to / (%s)\n", strerror(errno));
		exit (1);
	}
*/

	Log->Write("Daemonization finished.\n");

	delete Log;
}

void InitDirectories(void) {
	mkdir("logs", S_IWUSR | S_IRUSR | S_IXUSR); 
 	unlink("logs/thread.log");
	unlink("logs/message.log");
}

void SigHandler(int Signal) {
	switch (Signal) {
		case SIGTERM:
		case SIGINT:
			Globals->done = 1;
			break;
		case SIGPIPE:
			break;
		default:
			break;
	}
}

int Help(void) {
	printf("War-Net Help:\n");
	printf("  -d --daemon\n");
	printf("  -h --help\n");
	printf("  -p --port (port)\n");
	return 0;
}
