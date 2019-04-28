#include "Condition.h"

#define kNanoSecondsPerSecond 1000000000

using namespace muduo;

bool Condition::waitForSeconds(double seconds) {
	struct timespec abstime;
	int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

	clock_gettime(CLOCK_REALTIME, &abstime);
	abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec+nanoseconds)/kNanoSecondsPerSecond);
	abstime.tv_nsec = static_cast<long>((abstime.tv_nsec+nanoseconds)%kNanoSecondsPerSecond);
	MutexLock::UnassignGuard ug(mutex_);
	return pthread_cond_timedwait(&cond_, mutex_.getThreadMutex(), &abstime);
}
