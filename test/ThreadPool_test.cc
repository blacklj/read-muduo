#include "base/ThreadPool.h"
#include "base/CurrentThread.h"
#include "base/CountDownLatch.h"

#include <stdio.h>
#include <unistd.h>

using namespace muduo;

void print() {
	printf("tid=%d\n", CurrentThread::tid());
}

void printString(std::string &str) {
	printf("%s\n", str.c_str());
	usleep(100*1000);
}

void test(int maxSize) {
	printf("Test ThreadPool with max queue size %d\n", maxSize);
	ThreadPool pool("MainThreadPool");
	pool.setMaxQueueSize(maxSize);
	pool.start(5);

	printf("adding\n");
	pool.run(print);
	pool.run(print);
	for (int i = 0; i < 100; ++i) {
		char buf[32];
		snprintf(buf, sizeof(buf), "task %d", i);
		pool.run(std::bind(printString, std::string(buf)));
	}
	printf("done\n");

	CountDownLatch latch(1);
	pool.run(std::bind(&CountDownLatch::countDown, &latch));
	latch.wait();
	pool.stop();
}

int main() {
	test(0);
	test(1);
	test(5);
	test(10);
	test(50);
	return 0;
}
