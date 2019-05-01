#include "base/Singleton.h"
#include "base/noncopyable.h"
#include "base/ThreadLocal.h"
#include "base/Thread.h"
#include "base/CurrentThread.h"

#include <stdio.h>
#include <unistd.h>

using namespace muduo;

class Test : noncopyable {
public:
	Test() {
		printf("tid=%d, constructing %p\n", CurrentThread::tid(), this);
	}
	~Test() {
		printf("tid=%d, destructing %p %s\n", CurrentThread::tid(), this, name_.c_str());
	}

	const std::string &name() const {
		return name_;
	}

	void setName(const std::string &name) {
		name_ = name;
	}

private:
	std::string name_;
};

void print() {
	printf("tid=%d, %p %p name=%s\n", CurrentThread::tid(),
		   &Singleton<ThreadLocal<Test>>::instance(),
		   &Singleton<ThreadLocal<Test>>::instance().value(),
		   Singleton<ThreadLocal<Test>>::instance().value().name().c_str());
}

void threadFunc(const char *changeTo) {
	print();
	Singleton<ThreadLocal<Test>>::instance().value().setName(changeTo);
	sleep(1);
	print();
}

int main() {
	Singleton<ThreadLocal<Test>>::instance().value().setName("main one");
	Thread t1(std::bind(threadFunc, "thread1"));
	Thread t2(std::bind(threadFunc, "thread2"));
	t1.start();
	t2.start();
	t1.join();
	print();
	t2.join();
	pthread_exit(0);
}
