#include "base/BoundedBlockingQueue.h"
#include "base/CurrentThread.h"
#include "base/CountDownLatch.h"
#include "base/Thread.h"
#include "base/Logger.h"

#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace muduo;

class Test {
public:
	Test(int numThreads) : latch_(numThreads), queue_(20) {
		for (int i = 0; i < numThreads; ++i) {
			char name[32];
			snprintf(name, sizeof(name), "work thread %d", i);
			threads_.emplace_back(new Thread(std::bind(&Test::threadFunc, this), std::string(name)));
		}
		for (auto &ptr : threads_)
			ptr->start();
	}

	void run(int times) {
		LOG_INFO << "waiting for count down latch";
		latch_.wait();
		for (int i = 0; i < times; ++i) {
			char buf[32];
			snprintf(buf, sizeof(buf), "hello %i", i);
			queue_.put(buf);
			LOG_INFO << "tid=" << CurrentThread::tid()
					 << " put data=" << buf 
					 << " size=" << queue_.size();
		}
	}

	void joinAll() {
		for (size_t i = 0; i < threads_.size(); ++i) {
			queue_.put("stop");
		}

		for (auto &ptr : threads_)
			ptr->join();
	}

private:
	void threadFunc() {
		LOG_INFO << "tid=" << CurrentThread::tid() << ' ' << CurrentThread::name() << "start";
		latch_.countDown();
		bool running = true;
		while (running) {
			std::string s(queue_.take());
			LOG_INFO << "tid=" << CurrentThread::tid() << " get data=" << s << " size=" << queue_.size();
			running = (s != "stop");
		}
		LOG_INFO << "tid=" << CurrentThread::tid() << ' ' << CurrentThread::name() << " stop";
	}

	CountDownLatch latch_;
	BoundedBlockingQueue<std::string> queue_;
	std::vector<std::unique_ptr<Thread>> threads_;
};

int main() {
	LOG_INFO << "pid=" << getpid() << ' ' << "tid=" << CurrentThread::tid();
	Test t(5);
	t.run(100);
	t.joinAll();
}
