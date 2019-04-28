#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__

#include <string>
#include <algorithm>
#include <stdint.h>

namespace muduo {

class Timestamp {
public:
	Timestamp() : microSecondsSinceEpoch_(0) {}
	explicit Timestamp(int64_t microSecondsSinceEpoch)
		: microSecondsSinceEpoch_(microSecondsSinceEpoch) {
	}

	inline bool operator<(Timestamp t) {
		return microSecondsSinceEpoch_ < t.microSecondsSinceEpoch_;
	}
	inline bool operator>(Timestamp t) {
		return microSecondsSinceEpoch_ > t.microSecondsSinceEpoch_;
	}
	
	inline bool operator==(Timestamp t) {
		return microSecondsSinceEpoch_ == t.microSecondsSinceEpoch_;
	}
	inline bool operator!=(Timestamp t) {
		return microSecondsSinceEpoch_ != t.microSecondsSinceEpoch_;
	}

	int64_t microSecondsSinceEpoch() const {
		return microSecondsSinceEpoch_;
	}
	time_t secondsSinceEpoch() const {
		return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
	}
	std::string toString() const;
	std::string toFormatString(bool showMicrosecond) const;
	bool valid() {
		return microSecondsSinceEpoch_ > 0;
	}
	void swap(Timestamp &that) {
		std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
	}

	static Timestamp now();
	static Timestamp invalid() {
		return Timestamp();
	}
	static Timestamp formatUnixTime(time_t t, int microSeconds) {
		return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microSeconds);
	}
	static Timestamp formatUnixTime(time_t t) {
		return formatUnixTime(t, 0);
	}

	static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
	int64_t microSecondsSinceEpoch_;
};

inline double timeDifference(Timestamp h, Timestamp l) {
	return static_cast<double>(h.microSecondsSinceEpoch() - l.microSecondsSinceEpoch()) / Timestamp::kMicroSecondsPerSecond;
}

inline Timestamp addTime(Timestamp t, double seconds) {
	return Timestamp(static_cast<int64_t>(seconds)*Timestamp::kMicroSecondsPerSecond + t.microSecondsSinceEpoch());
}

}

#endif
