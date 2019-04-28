#ifndef __THREAD_H__
#define __THREAD_H__

#include "Atomic.h"
#include "noncopyable.h"
#include "CountDownLatch.h"

#include <functional>
#include <pthread.h>
#include <memory>

namespace muduo {

class Thread : noncopyable {
public:
	typedef std::function<void ()> ThreadFunc;

	explicit Thread(ThreadFunc func, const std::string &name = std::string());
	~Thread();

	void start();
	int join();
	bool started() {
		return started_;
	}
	pid_t tid() {
		return tid_;
	}
	static int numCreated() {
		return numCreated_.get();
	}

private:
	void setDefaultName();

	std::string name_;
	bool started_;
	bool joined_;
	ThreadFunc func_;
	pthread_t threadId_;
	pid_t tid_;
	static AtomicInt32 numCreated_;
	CountDownLatch latch_;
};

}

#endif
