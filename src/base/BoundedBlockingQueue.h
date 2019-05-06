#ifndef __BOUNDED_BLOCKING_QUEUE_H__
#define __BOUNDED_BLOCKING_QUEUE_H__

#include "noncopyable.h"
#include "Mutex.h"
#include "Condition.h"

#include <assert.h>
#include <boost/circular_buffer.hpp>

namespace muduo {

template <typename T>
class BoundedBlockingQueue : noncopyable {
public:
	BoundedBlockingQueue(int maxSize)
		: mutex_(),
		  notFull_(mutex_),
		  notEmpty_(mutex_),
		  queue_(maxSize) {
	}

	void put(const T &x) {
		MutexLockGuard lock(mutex_);
		while (queue_.full()) {
			notFull_.wait();
		}
		assert(!queue_.full());
		queue_.push_back(x);
		notEmpty_.notify();
	}

	void put(T &&x) {
		MutexLockGuard lock(mutex_);
		while (queue_.full()) {
			notFull_.wait();
		}
		assert(!queue_.full());
		queue_.push_back(std::move(x));
		notEmpty_.notify();
	}

	T take() {
		MutexLockGuard lock(mutex_);
		while (queue_.empty()) {
			notEmpty_.wait();
		}
		assert(!queue_.empty());
		T t(std::move(queue_.front()));
		queue_.pop_front();
		notFull_.notify();
		return std::move(t);
	}

	bool full() const {
		MutexLockGuard lock(mutex_);
		return queue_.full();
	}

	bool empty() const {
		MutexLockGuard lock(mutex_);
		return queue_.empty();
	}

	size_t size() const {
		MutexLockGuard lock(mutex_);
		return queue_.size();
	}

	size_t capacity() const {
		MutexLockGuard lock(mutex_);
		return queue_.capacity();
	}

private:
	mutable MutexLock mutex_;
	Condition notFull_;
	Condition notEmpty_;
	boost::circular_buffer<T> queue_;
};

}

#endif
