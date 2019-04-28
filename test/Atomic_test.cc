#include "base/Atomic.h"

#include <assert.h>

using namespace muduo;

int main() {
	AtomicInt32 val32;
	assert(val32.get() == 0);
	val32.increment();
	assert(val32.get() == 1);
	val32.decrement();
	assert(val32.get() == 0);
	assert(val32.getAndAdd(1) == 0);
	assert(val32.get() == 1);
	assert(val32.addAndGet(1) == 2);
	val32.getAndSet(2);
	assert(val32.get() == 2);

	AtomicInt64 val64;
	assert(val64.get() == 0);
	val64.increment();
	assert(val64.get() == 1);
	val64.decrement();
	assert(val64.get() == 0);
	assert(val64.getAndAdd(1) == 0);
	assert(val64.get() == 1);
	assert(val64.addAndGet(1) == 2);
	val64.getAndSet(2);
	assert(val64.get() == 2);

	return 0;
}
