#include "base/LogStream.h"

#include <assert.h>
#include <string>
#include <limits>

using namespace muduo;

int main() {
	LogStream os;
	const LogStream::Buffer &buf = os.buffer();
	assert(buf.toString() == std::string(""));
	
	os << true;
	assert(buf.toString() == std::string("1"));
	os << '\n';
	assert(buf.toString() == std::string("1\n"));
	os << false;
	assert(buf.toString() == std::string("1\n0"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << 1;
	assert(buf.toString() == std::string("1"));
	os << 0;
	assert(buf.toString() == std::string("10"));
	os << -1;
	assert(buf.toString() == std::string("10-1"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << -2147483647;
	assert(buf.toString() == std::string("-2147483647"));
	os << static_cast<int>(-2147483647 - 1);
	assert(buf.toString() == std::string("-2147483647-2147483648"));
	os << ' ';
	os << 2147483647;
	assert(buf.toString() == std::string("-2147483647-2147483648 2147483647"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<int16_t>::min();
	assert(buf.toString() == std::string("-32768"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<int16_t>::max();
	assert(buf.toString() == std::string("32767"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<uint16_t>::min();
	assert(buf.toString() == std::string("0"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<uint16_t>::max();
	assert(buf.toString() == std::string("65535"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<int32_t>::min();
	assert(buf.toString() == std::string("-2147483648"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<int32_t>::max();
	assert(buf.toString() == std::string("2147483647"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<uint32_t>::min();
	assert(buf.toString() == std::string("0"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<uint32_t>::max();
	assert(buf.toString() == std::string("4294967295"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<int64_t>::min();
	assert(buf.toString() == std::string("-9223372036854775808"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<int64_t>::max();
	assert(buf.toString() == std::string("9223372036854775807"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<uint64_t>::min();
	assert(buf.toString() == std::string("0"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << std::numeric_limits<uint64_t>::max();
	assert(buf.toString() == std::string("18446744073709551615"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << 0.0;
	assert(buf.toString() == std::string("0"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << 1.0;
	assert(buf.toString() == std::string("1"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << 0.1;
	assert(buf.toString() == std::string("0.1"));

	double a = 0.01;
	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << a;
	assert(buf.toString() == std::string("0.01"));

	a = -0.1234567;
	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << a;
	assert(buf.toString() == std::string("-0.1234567"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << static_cast<void *>(0);
	assert(buf.toString() == std::string("0x0"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << reinterpret_cast<void *>(8888);
	assert(buf.toString() == std::string("0x22B8"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << "muduo test";
	assert(buf.toString() == std::string("muduo test"));

	os.resetBuffer();
	assert(buf.toString() == std::string(""));
	os << Fmt("%4.2f", 1.2) << Fmt("%4d", 43);
	assert(buf.toString() == std::string("1.20  43"));

	return 0;
}
