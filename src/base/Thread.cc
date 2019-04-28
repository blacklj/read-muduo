#include "Thread.h"
#include "CurrentThread.h"
#include "Exception.h"
#include "Timestamp.h"

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <assert.h>
#include <sys/prctl.h>

using namespace muduo;

pid_t gettid() {
	return static_cast<pid_t> (syscall(SYS_gettid));
}

void CurrentThread::cacheTid() {
	if (t_cachedTid == 0) {
		t_cachedTid = gettid();
		t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%5d", t_cachedTid);
	}
}

bool CurrentThread::isMainThread() {
	return tid() == getpid();
}

void CurrentThread::sleepUsec(int64_t usec) {
	struct timespec ts = {0, 0};

	ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
	ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
	nanosleep(&ts, NULL);
}

void afterFork() {
	CurrentThread::t_cachedTid = 0;
	CurrentThread::t_threadName = "main";
	CurrentThread::tid();
};

class ThreadNameInitializer {
public:
	ThreadNameInitializer() {
		CurrentThread::t_threadName = "main";
		CurrentThread::tid();
		pthread_atfork(NULL,NULL,&afterFork);
	};
};

ThreadNameInitializer init;

struct ThreadData {
	typedef Thread::ThreadFunc ThreadFunc;

	ThreadFunc func_;
	std::string name_;
	pid_t *tid_;
	CountDownLatch *latch_;

	ThreadData(ThreadFunc func,
			const std::string &name,
			pid_t *tid,
			CountDownLatch *latch)
	: func_(func),
	  name_(name),
	  tid_(tid),
	  latch_(latch) {}

	void runInThread() {
		*tid_ = CurrentThread::tid();
		tid_ = NULL;
		latch_->countDown();
		latch_ = NULL;

		CurrentThread::t_threadName = name_.empty()?"muduoThread":name_.c_str();
		::prctl(PR_SET_NAME, CurrentThread::t_threadName);

		try {
			func_();
			CurrentThread::t_threadName = "finished";
		} catch (const Exception &ex) {
			CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
			fprintf(stderr, "reason: %s\n", ex.what());
			fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
			abort();
		} catch (const std::exception &ex) {
			CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
			fprintf(stderr, "reason: %s\n", ex.what());
			abort();
		} catch (...) {
			CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
			throw;
		}
	}
};

void *startThread(void *arg) {
	ThreadData *data = static_cast<ThreadData *>(arg);
	data->runInThread();
	delete data;
	return NULL;
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(ThreadFunc func, const std::string &name)
	: name_(name),
	  started_(false),
	  joined_(false),
	  func_(func),
	  threadId_(0),
	  tid_(0),
	  latch_(1) {
	setDefaultName();
}

Thread::~Thread() {
	if (started_ && !joined_)
		pthread_detach(threadId_);
}

void Thread::setDefaultName() {
	int num = numCreated_.incrementAndGet();
	if (name_.empty()) {
		char buf[32];
		snprintf(buf, sizeof(buf), "Thread%d", num);
		name_ = buf;
	}
}

void Thread::start() {
	assert(!started_);
	started_ = true;
	ThreadData *data = new ThreadData(func_,name_,&tid_,&latch_);
	if (pthread_create(&threadId_, NULL, &startThread, data)) {
		started_ = false;
		delete data;
		printf("Failed in pthread_create\n");
	} else {
		latch_.wait();
		assert(tid_ > 0);
	}
}

int Thread::join() {
	assert(started_);
	assert(!joined_);
	joined_ = true;
	return pthread_join(threadId_, NULL);
}
