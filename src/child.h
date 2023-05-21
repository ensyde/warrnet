#ifndef CHILD_H
#define CHILD_H

#include <list>
#include <map>
#include <string>

#include "clog.h"
#include "types.h"
#include "consts.h"
#include "client.h"
#include "threads.h"

using namespace std;

class CChild : public CThread {
	CLog *Log;
	
	CClient *ClientList[MAX_CLIENTS];
	unsigned ClientCount;

	int ChildNumber;

public:	
	int TriggerFD;

public:
	CChild(int ChildNumber);
   ~CChild(void);

	void Initialize(void);
	void Instance(void);		// Child loop

	void RemoveClient(unsigned Index);
	CClient *GetClient(unsigned Index);
	unsigned GetClientCount(void) const { return (ClientCount); }

	bool IsClient_Okay(CClient *Client);

	void Trigger(int FD);
};

#endif
