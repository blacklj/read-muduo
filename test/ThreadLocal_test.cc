#include "base/ThreadLocal.h"
#include "base/noncopyable.h"
#include "base/Thread.h"
#include "base/CurrentThread.h"

#include <stdio.h>
#include <string>

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

ThreadLocal<Test> testObj1;
ThreadLocal<Test> testObj2;

void print() {
	printf("tid=%d,obj1 %p name=%s\n",
		   CurrentThread::tid(),
		   &testObj1.value(),
		   testObj1.value().name().c_str());

	printf("tid=%d,obj2 %p name=%s\n",
		   CurrentThread::tid(),
		   &testObj2.value(),
		   testObj2.value().name().c_str());
}

void threadFunc() {
	print();
	testObj1.value().setName("changed 1");
	testObj2.value().setName("changed 2");
	print();
}

int main() {
	testObj1.value().setName("main one");
	print();
	Thread t1(threadFunc);
	t1.start();
	t1.join();
	testObj2.value().setName("main two");
	print();

	pthread_exit(0);
}
