#ifndef __THREAD_LOCAL_SINGLETON_H__
#define __THREAD_LOCAL_SINGLETON_H__

#include "noncopyable.h"

#include <assert.h>
#include <pthread.h>

namespace muduo {

template <typename T>
class ThreadLocalSingleton : noncopyable {
public:
	ThreadLocalSingleton() = delete;
	~ThreadLocalSingleton() = delete;

	static T &instance() {	// 每一线程的instance返回值都不一样
		if (!value_) {
			value_ = new T();
			deleter_.set(value_);
		}
		return *value_;
	}

	static T *pointer() {
		return value_;
	}

private:
	static void destructor(void *obj) {
		assert(obj == value_);
		typedef char T_must_be_complete_type[sizeof(T)==0?-1:1];
		T_must_be_complete_type dummy;
		(void)dummy;
		delete value_;
		value_ = NULL;
	}

	class Deleter {
	public:
		Deleter() {
			pthread_key_create(&key_, &ThreadLocalSingleton::destructor);
		}
		~Deleter() {
			pthread_key_delete(key_);
		}

		void set(T *obj) {
			assert(pthread_getspecific(key_) == NULL);
			pthread_setspecific(key_, obj);
		}

	private:
		pthread_key_t key_;
	};

	static __thread T *value_; // 析构时无法调用T的析构函数,利用辅助类Deleter释放内存
	static Deleter deleter_;
};

template <typename T>
__thread T *ThreadLocalSingleton<T>::value_ = 0;

template <typename T>
typename ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::deleter_;

}

#endif
