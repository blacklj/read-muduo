#include "base/ThreadLocalSingleton.h"
#include "base/CurrentThread.h"
#include "base/Thread.h"

#include <stdio.h>
#include <unistd.h>

using namespace muduo;

class Test : noncopyable {
public:
	Test() {
		printf("tid=%d, constructing %p\n", CurrentThread::tid(), this);
	}
	~Test() {
		printf("tid=%d, destructing %p\n", CurrentThread::tid(), this);
	}

	const std::string &name() {
		return name_;
	}
	void setName(const std::string &n) {
		name_ = n;
	}

private:
	std::string name_;
};

void threadFunc1(const char *changeTo) {
	printf("tid=%d, %p name=%s\n", CurrentThread::tid(),
		   &ThreadLocalSingleton<Test>::instance(),
		   ThreadLocalSingleton<Test>::instance().name().c_str());

	ThreadLocalSingleton<Test>::instance().setName(changeTo);

	printf("tid=%d, %p name=%s\n", CurrentThread::tid(),
		   &ThreadLocalSingleton<Test>::instance(),
		   ThreadLocalSingleton<Test>::instance().name().c_str());
	sleep(1);
}
void threadFunc2(const char *changeTo) {
	printf("tid=%d, %p name=%s\n", CurrentThread::tid(),
		   &ThreadLocalSingleton<Test>::instance(),
		   ThreadLocalSingleton<Test>::instance().name().c_str());

	ThreadLocalSingleton<Test>::instance().setName(changeTo);

	printf("tid=%d, %p name=%s\n", CurrentThread::tid(),
		   &ThreadLocalSingleton<Test>::instance(),
		   ThreadLocalSingleton<Test>::instance().name().c_str());
}

int main() {
	ThreadLocalSingleton<Test>::instance().setName("main one");
	Thread t1(std::bind(threadFunc1, "thread1"));
	Thread t2(std::bind(threadFunc2, "thread2"));
	t1.start();
	t2.start();
	t1.join();

	printf("tid=%d, %p name=%s\n", CurrentThread::tid(),
		   &ThreadLocalSingleton<Test>::instance(),
		   ThreadLocalSingleton<Test>::instance().name().c_str());

	t2.join();

	pthread_exit(0);
}
