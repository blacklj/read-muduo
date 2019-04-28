#include "CountDownLatch.h"

using namespace muduo;

CountDownLatch::CountDownLatch(int count)
	: count_(count),
	mutex_(),
	cond_(mutex_) {}

void CountDownLatch::wait() {
	MutexLockGuard lock(mutex_);
	while (count_ > 0)
		cond_.wait();
}

void CountDownLatch::countDown() {
	MutexLockGuard lock(mutex_);
	--count_;
	if (count_ == 0)
		cond_.notifyAll();
}

int CountDownLatch::getCount() const {
	MutexLockGuard lock(mutex_);
	return count_;
}
