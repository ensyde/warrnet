#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>
#include <unistd.h>

class CThread {
	pthread_t ThreadHandle;
	pthread_mutex_t ThreadMutex;
public:
	CThread(void);
   virtual ~CThread(void);

	virtual void Instance(void) = 0;

	void Start(void *(*func)(void *), void *Arg);
	void Stop(void);
	void Join(void);

	void Lock(void);
	void Unlock(void);
};

#endif
