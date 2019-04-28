#include "base/Thread.h"
#include "base/CurrentThread.h"

#include <unistd.h>

using namespace muduo;

void mysleep(int seconds) {
	timespec t = {seconds, 0};
	nanosleep(&t, NULL);
}

void threadFunc() {
	printf("tid = %d\n", CurrentThread::tid());
}

void threadFunc2(int arg) {
	printf("tid = %d, arg = %d\n", CurrentThread::tid(), arg);
}

void threadFunc3() {
	printf("tid = %d\n", CurrentThread::tid());
	mysleep(1);
}

class Foo {
public:
	Foo(int x) : x_(x) {}

	void memberFunc() {
		printf("tid = %d, Foo::x_ = %f\n", CurrentThread::tid(), x_);
	}

	void memberFunc2(std::string &text) {
		printf("tid = %d, Foo::x_ = %f, text = %s\n", CurrentThread::tid(), x_, text.c_str());
	}

private:
	double x_;
};

int main() {
	printf("pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
	Thread t1(threadFunc);
	t1.start();
	printf("t1.tid = %d\n", t1.tid());
	t1.join();

	Thread t2(std::bind(threadFunc2, 42), "thread for free function with arg");
	t2.start();
	printf("t2.tid = %d\n", t2.tid());
	t2.join();

	Foo foo(33.3);
	Thread t3(std::bind(&Foo::memberFunc, &foo), "thread for member func");
	t3.start();
	printf("t3.tid = %d\n", t3.tid());

	Thread t4(std::bind(&Foo::memberFunc2, &foo, std::string("rihnus")));
	t4.start();
	printf("t4.tid = %d\n", t4.tid());

	{
		Thread t5(threadFunc3);
		t5.start();
	}
	mysleep(2);
	{
		Thread t6(threadFunc3);
		t6.start();
		mysleep(2);
	}
	sleep(2);
	printf("number of created threads %d\n", Thread::numCreated());

	return 0;
}
