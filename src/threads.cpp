#include "threads.h"

CThread::CThread(void) {
	pthread_mutex_init(&ThreadMutex, 0);
}

CThread::~CThread(void) {
	Stop();	// needed?
}

void CThread::Start(void *(*func)(void *), void *Arg) {
	pthread_create(&ThreadHandle, 0, func, Arg);
}

void CThread::Stop(void) {
	pthread_exit(0);
}

//
//  This function will not return until the thread exits.
//
void CThread::Join(void) {
	pthread_join(ThreadHandle, NULL);
}

void CThread::Lock(void) {
	pthread_mutex_lock(&ThreadMutex);
}

void CThread::Unlock(void) {
	pthread_mutex_unlock(&ThreadMutex);
}
