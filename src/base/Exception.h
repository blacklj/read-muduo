#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <string>
#include <exception>

namespace muduo {

class Exception : public std::exception {
public:
	Exception(std::string what);
	~Exception() noexcept override = default;

	const char *what() const noexcept override {
		return message_.c_str();
	}

	const char *stackTrace() const noexcept {
		return stack_.c_str();
	}

private:
	std::string message_;
	std::string stack_;
};

}

#endif
