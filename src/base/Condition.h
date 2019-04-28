#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "noncopyable.h"
#include "Mutex.h"

#include <pthread.h>

namespace muduo {

class Condition : noncopyable {
public:
	Condition(MutexLock &mutex)
		: mutex_(mutex) {
		pthread_cond_init(&cond_, NULL);
	}

	~Condition() {
		pthread_cond_destroy(&cond_);
	}

	void wait() {
		MutexLock::UnassignGuard ug(mutex_);
		pthread_cond_wait(&cond_, mutex_.getThreadMutex());
	}

	bool waitForSeconds(double seconds);

	void notify() {
		pthread_cond_signal(&cond_);
	}

	void notifyAll() {
		pthread_cond_broadcast(&cond_);
	}

private:
	MutexLock &mutex_;
	pthread_cond_t cond_;
};

}

#endif
