#include "base/Logger.h"
#include "base/ThreadPool.h"

#include <unistd.h>

using namespace muduo;

int g_total;
FILE *g_file;

void dummyOutput(const char *msg, int len) {
	g_total += len;
	if (g_file)
		fwrite(msg, 1, len, g_file);
}

void bench(const char *type) {
	Logger::setOutput(dummyOutput);
	Timestamp start(Timestamp::now());
	g_total = 0;

	const bool kLongLog = false;
	std::string longStr(3000, 'X');
	longStr += " ";
	std::string empty = " ";
	int n = 1000 * 1000;
	for (int i = 0; i < n; ++i) {
		LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz"
			<< (kLongLog ? longStr : empty)
			<< i;
	}

	Timestamp end(Timestamp::now());
	double seconds = timeDifference(end, start);
	printf("%12s: %f seconds, %d bytes, %10.2f msg/s, %.2f MiB/s\n",
			type, seconds, g_total, n / seconds, g_total / seconds / (1024 * 1024));
}

void logInThread() {
	LOG_INFO << "logInThread()";
	usleep(1000);
}

int main() {
	LOG_INFO << "main()";
	getppid();
	ThreadPool pool("pool");
	pool.start(5);
	pool.run(logInThread);
	pool.run(logInThread);
	pool.run(logInThread);
	pool.run(logInThread);
	pool.run(logInThread);

	LOG_TRACE << "trace";
	LOG_DEBUG << "debug";
	LOG_INFO << "Hello";
	LOG_WARN << "World";
	LOG_ERROR << "Error";
	LOG_INFO << sizeof(muduo::Logger);
	LOG_INFO << sizeof(muduo::LogStream);
	LOG_INFO << sizeof(muduo::Fmt);
	LOG_INFO << sizeof(muduo::LogStream::Buffer);

	char buffer[64*1024];
	g_file = fopen("/tmp/log", "w");
	setbuffer(g_file, buffer, sizeof(buffer));
	bench("/tmp/log");
	fclose(g_file);

	return 0;
}
