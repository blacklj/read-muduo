#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"

#include <vector>
#include <deque>
#include <string>

namespace muduo {

class ThreadPool : noncopyable {
public:
	typedef std::function<void ()> Task;

	explicit ThreadPool(const std::string &name = std::string("ThreadPool"));
	~ThreadPool();

	const std::string &name() const {
		return name_;
	}
	void setMaxQueueSize(int maxSize) {
		maxQueueSize_ = maxSize;
	}
	void setThreadInitCallback(const Task &cb) {
		threadInitCallback_ = cb;
	}
	size_t queueSize() const ;

	void start(int numThreads);
	void stop();
	void run(Task f);

private:
	bool isFull() const;
	void runInThread();
	Task take();

	std::string name_;
	bool running_;
	mutable MutexLock mutex_;
	Condition notFull_;
	Condition notEmpty_;
	std::vector<std::unique_ptr<Thread>> threads_;
	std::deque<Task> queue_;
	size_t maxQueueSize_;
	Task threadInitCallback_;
};

}

#endif
