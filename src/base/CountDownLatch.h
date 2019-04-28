#ifndef __COUNTDOWN_LATCH_H__
#define __COUNTDOWN_LATCH_H__

#include "noncopyable.h"
#include "Mutex.h"
#include "Condition.h"

namespace muduo {

class CountDownLatch : noncopyable {
public:
	explicit CountDownLatch(int count);
	void wait();
	void countDown();
	int getCount() const;

private:
	int count_;
	mutable MutexLock mutex_;
	Condition cond_;
};

}

#endif
