#include "Timestamp.h"

#include <stdio.h>
#include <inttypes.h>
#include <sys/time.h>

using namespace muduo;

Timestamp Timestamp::now() {
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return Timestamp(static_cast<int64_t>(tv.tv_sec*kMicroSecondsPerSecond + tv.tv_usec));
}

std::string Timestamp::toString() const {
	char buf[32];
	int64_t seconds, microSeconds;

	seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
	microSeconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
	snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microSeconds);
	return buf;
}

std::string Timestamp::toFormatString(bool showMicrosecond) const {
	char buf[32];
	struct tm t;
	time_t seconds;

	seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
	gmtime_r(&seconds, &t);
	if (showMicrosecond) {
		int microSeconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d", t.tm_year+1900,t.tm_mon+1,t.tm_mday,
				 t.tm_hour, t.tm_min, t.tm_sec, microSeconds);
	} else {
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d", t.tm_year+1900,t.tm_mon+1,t.tm_mday,
				 t.tm_hour, t.tm_min, t.tm_sec);
	}

	return buf;
}
