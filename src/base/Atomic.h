#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#include <stdint.h>

namespace muduo {

template <typename T>
class Atomic {
public:
	Atomic() : value_(0) {}

	Atomic(const Atomic &that) : value_(that.get()) {}

	Atomic &operator=(const Atomic &that) {
		getAndAdd(that.get());
		return *this;
	}

	T getAndAdd(T x) {
		return __sync_fetch_and_add(&value_, x);
	}

	T addAndGet(T x) {
		return getAndAdd(x) + x;
	}

	T incrementAndGet() {
		return addAndGet(1);
	}

	void increment() {
		incrementAndGet();
	}

	void decrement() {
		getAndAdd(-1);
	}

	T get() {
		return __sync_val_compare_and_swap(&value_, 0, 0);
	}

	T getAndSet(T x) {
		return __sync_lock_test_and_set(&value_, x);
	}

private:
	volatile T value_;
};

typedef Atomic<int32_t> AtomicInt32;
typedef Atomic<int64_t> AtomicInt64;

}

#endif
