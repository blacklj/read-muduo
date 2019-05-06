#ifndef __LOG_STREAM_H__
#define __LOG_STREAM_H__

#include "noncopyable.h"
#include "StringPiece.h"

#include <string.h>
#include <string>

namespace muduo {

const int kSmallBuffer = 4000;
const int kLargeBuffer = 1000 * 4000;

template <int SIZE>
class FixedBuffer : noncopyable {
public:
	FixedBuffer() : cur_(data_) {
		setCookie(cookieStart);
	}
	~FixedBuffer() {
		setCookie(cookieEnd);
	}

	void append(const char *buf, size_t len) {
		if (static_cast<size_t>(avail()) > len) {
			memcpy(cur_, buf, len);
			cur_ += len;
		}
	}
	const char *data() const {
		return data_;
	}
	int length() const {
		return static_cast<int>(cur_ - data_);
	}
	char *current() const {
		return cur_;
	}
	int avail() const {
		return static_cast<int>(end() - cur_);
	}
	void add(int len) {
		cur_ += len;
	}
	void reset() {
		cur_ = data_;
	}
	void bzero() {
		memset(data_, 0, sizeof(data_));
	}

	const char *debugString();
	void setCookie(void (*cookie)()) {
		cookie_ = cookie;
	}
	std::string toString() const {
		return std::string(data_, length());
	}
	StringPiece toStringPiece() const {
		return StringPiece(data_, length());
	}

private:
	const char *end() const {
		return data_ + sizeof(data_);
	}
	static void cookieStart();
	static void cookieEnd();

	void (*cookie_)();
	char data_[SIZE];
	char *cur_;
};

class LogStream : noncopyable {
public:
	typedef FixedBuffer<kSmallBuffer> Buffer;

	LogStream &operator<<(bool v) {
		buffer_.append(v?"1":"0", 1);
		return *this;
	}
	LogStream &operator<<(int v);
	LogStream &operator<<(unsigned int v);
	LogStream &operator<<(short v);
	LogStream &operator<<(unsigned short v);
	LogStream &operator<<(long v);
	LogStream &operator<<(unsigned long v);
	LogStream &operator<<(long long v);
	LogStream &operator<<(unsigned long long v);
	LogStream &operator<<(float v) {
		*this << static_cast<double>(v);
		return *this;
	}
	LogStream &operator<<(double v);

	LogStream &operator<<(char v) {
		buffer_.append(&v, 1);
		return *this;
	}
	LogStream &operator<<(const char *v) {
		if (v) {
			buffer_.append(v, strlen(v));
		} else {
			buffer_.append("(null)", 6);
		}
		return *this;
	}
	LogStream &operator<<(const unsigned char *v) {
		return operator<<(reinterpret_cast<const char *>(v));
	}
	LogStream &operator<<(const std::string &v) {
		buffer_.append(v.data(), v.size());
		return *this;
	}
	LogStream &operator<<(const StringPiece &v) {
		buffer_.append(v.data(), v.size());
		return *this;
	}
	LogStream &operator<<(const Buffer &v) {
		*this << v.toStringPiece();
		return *this;
	}
	LogStream &operator<<(const void *p);

	void append(const char *data, int len) {
		buffer_.append(data, len);
	}
	const Buffer &buffer() const {
		return buffer_;
	}
	void resetBuffer() {
		buffer_.reset();
	}

private:
	void staticCheck();
	
	template <typename T>
	void formatInteger(T);

	Buffer buffer_;
	static const int kMaxNumericSize = 32;
};

class Fmt {
public:
	template <typename T>
	Fmt(const char *fmt, T val);

	const char *data() const {
		return buf_;
	}
	int length() const {
		return length_;
	}

private:
	char buf_[32];
	int length_;
};

inline LogStream &operator<<(LogStream &x, const Fmt &fmt) {
	x.append(fmt.data(), fmt.length());
	return x;
}

std::string formatSI(int64_t n);
std::string formatIEC(int64_t n);

}

#endif
