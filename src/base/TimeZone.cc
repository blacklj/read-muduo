#include "TimeZone.h"

using namespace muduo;

struct tm TimeZone::toLocalTime(time_t secondsSinceEpoch) const {
	struct tm localTime;
	return localTime;
}
