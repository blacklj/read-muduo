#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "noncopyable.h"
#include "CurrentThread.h"

#include <pthread.h>
#include <assert.h>

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (__builtin_expect(errnum != 0, 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

namespace muduo {

class MutexLock : noncopyable {
public:
	MutexLock() {
		pthread_mutex_init(&mutex_, NULL);
	}
	~MutexLock() {
		pthread_mutex_destroy(&mutex_);
	}

	void lock() {
		pthread_mutex_lock(&mutex_);
		assignHolder();
	}
	void unlock() {
		unassignHolder();
		pthread_mutex_unlock(&mutex_);
	}

	pthread_mutex_t *getThreadMutex() {
		return &mutex_;
	}

	bool isLockedByThisThread() const {
		return holder_ == CurrentThread::tid();
	}

	void assertLocked() {
		assert(isLockedByThisThread());
	}

private:
	friend class Condition;

	class UnassignGuard {
	public:
		explicit UnassignGuard(MutexLock &mutex)
			: owner_(mutex) {
			owner_.unassignHolder();
		}
		~UnassignGuard() {
			owner_.assignHolder();
		}

	private:
		MutexLock &owner_;
	};

	void unassignHolder() {
		holder_ = 0;
	}

	void assignHolder() {
		holder_ = CurrentThread::tid();
	}

	pid_t holder_;
	pthread_mutex_t mutex_;
};

class MutexLockGuard : noncopyable {
public:
	MutexLockGuard(MutexLock &mutex) : mutex_(mutex) {
		mutex_.lock();
	}
	~MutexLockGuard() {
		mutex_.unlock();
	}

private:
	MutexLock &mutex_;
};

}

#endif
