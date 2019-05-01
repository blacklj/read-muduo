#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "noncopyable.h"

#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

namespace muduo {

// SFINAE
// 如果T没有定义no_destroy成员函数，匹配int32_t test(...), value==false
template <typename T>
struct has_no_destroy {
	template <typename C> static char test(decltype(&C::no_destroy));
	template <typename C> static int32_t test(...);
	const static bool value = sizeof(test<T>(0)) == 1;
};

template <typename T>
class Singleton : noncopyable {
public:
	Singleton() = delete;
	~Singleton() = delete;

	static T &instance() {
		pthread_once(&once_, &Singleton::init);
		assert(value_ != NULL);
		return *value_;
	}

private:
	static void init() {
		value_ = new T();
		if (!has_no_destroy<T>::value)
			atexit(destroy);
	}

	static void destroy() {
		typedef char T_must_be_complete_type[sizeof(T)==0?-1:1];
		T_must_be_complete_type dummy;
		(void)dummy;
		delete value_;
		value_ = NULL;
	}

private:
	static pthread_once_t once_;
	static T *value_;
};

template <typename T>
pthread_once_t Singleton<T>::once_ = PTHREAD_ONCE_INIT;

template <typename T>
T *Singleton<T>::value_ = NULL;

}

#endif
