#ifndef __FILE_UTIL_H__
#define __FILE_UTIL_H__

#include "noncopyable.h"
#include "StringPiece.h"

#include <sys/types.h>  // for off_t

namespace muduo {

class AppendFile : noncopyable {
public:
	explicit AppendFile(StringArg name);
	~AppendFile();

	void append(const char *content, size_t len);
	void flush();
	off_t writtenBytes() const {
		return writtenBytes_;
	}

private:
	size_t write(const char *content, size_t len);

	FILE *fp_;
	char buf_[64*1024];
	off_t writtenBytes_;
};

class ReadSmallFile : noncopyable {
public:
	ReadSmallFile(StringArg name);
	~ReadSmallFile();

	template <typename T>
	int readToString(int maxSize, T *content, int64_t *fileSize, 
			int64_t *modifyTime, int64_t *createTime);
	int readToBuffer(int *size);
	const char *buffer() const {
		return buf_;
	}

	const static int kBufferSize = 64 * 1024;

private:
	int fd_;
	int err_;
	char buf_[kBufferSize];
};

template <typename T>
int readFile(StringArg name, 
		int maxSize,
		T *content, 
		int64_t *fileSize = NULL,
		int64_t *modifyTime = NULL, 
		int64_t *createTime = NULL) {
	ReadSmallFile file(name);
	return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}

}

#endif
