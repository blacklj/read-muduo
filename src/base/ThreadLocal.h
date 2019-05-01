#ifndef __THREAD_LOCAL_H__
#define __THREAD_LOCAL_H__

#include "noncopyable.h"
#include "Mutex.h"

#include <pthread.h>

namespace muduo {

template <typename T>
class ThreadLocal : noncopyable {
public:
	ThreadLocal() {
		MCHECK(pthread_key_create(&key_, &ThreadLocal::destructor));
	}
	~ThreadLocal() {
		pthread_key_delete(key_);
	}

	T &value() {
		T *old = static_cast<T *>(pthread_getspecific(key_));
		if (!old) {
			T *newObj = new T();
			pthread_setspecific(key_, newObj);
			old = newObj;
		}
		return *old;
	}

private:
	// pthread_exit时该函数才会被调用，return或exit不会调用该函数
	static void destructor(void *x) {
		T *obj = static_cast<T *>(x);
		typedef char T_must_be_complete_type[sizeof(T)==0?-1:1];
		T_must_be_complete_type dummy;
		(void) dummy;
		delete obj;
	}

	pthread_key_t key_;
};

}

#endif
