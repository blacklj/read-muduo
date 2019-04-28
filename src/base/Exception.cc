#include "Exception.h"
#include "CurrentThread.h"

using namespace muduo;

Exception::Exception(std::string what)
	: message_(std::move(what)),
	  stack_(CurrentThread::stackTrace(false)) {
	  }
