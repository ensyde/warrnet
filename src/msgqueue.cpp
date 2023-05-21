#include "msgqueue.h"

CMsgQueue::CMsgQueue( int limit) {
	msgqueue = new std::queue<SMsgQueue *>;
}

CMsgQueue::~CMsgQueue( void) {
	while (!msgqueue->empty()) {
		SMsgQueue *tmp = msgqueue->front();
		msgqueue->pop();
		Destroy( tmp);
	}
	
	delete msgqueue;
}

void CMsgQueue::OnJoin( CUser *user) {
	SMsgQueue *tmp = new SMsgQueue;
	tmp->type = ONJOIN;
	tmp->data = (void *)user;
	msgqueue->push( tmp);
}

void CMsgQueue::Destroy( SMsgQueue *obj) {
	switch (obj->type) {
		default:
			delete obj;
	}
}
