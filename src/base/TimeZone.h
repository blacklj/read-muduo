#ifndef __TIME_ZONE_H__
#define __TIME_ZONE_H__

#include <time.h>

namespace muduo {

class TimeZone {
public:
	bool valid() const {
		return false;
	}
	struct tm toLocalTime(time_t secondsSinceEpoch) const;
};

}

#endif
