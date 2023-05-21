#ifndef MSGQUEUE_H
#define MSGQUEUE_H

#include <queue>
#include "user.h"

struct SMsgQueue {
	int  type;
	void *data;
};

class CMsgQueue {
private:
	std::queue <SMsgQueue *> *msgqueue;
	int limit;

	void Destroy( SMsgQueue *obj);
public:
	CMsgQueue( int limit);
	~CMsgQueue( void);

	void OnJoin( CUser *user);

};

const int ONJOIN = 1;

#endif
