#include "base/Singleton.h"
#include "base/CurrentThread.h"
#include "base/Thread.h"

#include <stdio.h>

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

class TestNoDestroy : noncopyable {
public:
	void no_destroy();
	TestNoDestroy() {
		printf("tid=%d, constructing TestNoDestroy %p\n", CurrentThread::tid(), this);
	}
	~TestNoDestroy() {
		printf("tid=%d, destructing TestNoDestroy %p\n", CurrentThread::tid(), this);
	}
};

void threadFunc() {
	printf("tid=%d, %p name=%s\n", CurrentThread::tid(),
		   &Singleton<Test>::instance(),
		   Singleton<Test>::instance().name().c_str());
	Singleton<Test>::instance().setName("only one, changed");
}

int main() {
	Singleton<Test>::instance().setName("only one");
	Thread t1(threadFunc);
	t1.start();
	t1.join();

	printf("tid=%d, %p name=%s\n", CurrentThread::tid(),
		   &Singleton<Test>::instance(),
		   Singleton<Test>::instance().name().c_str());

	Singleton<TestNoDestroy>::instance();
	printf("with valgrind, you should see %zd-bytes memory leak.\n", sizeof(TestNoDestroy));

	Singleton<int>::instance();

	return 0;
}
