#ifndef LIBEVENTPP_UTIL_TIMESTAMP_INL_
#define LIBEVENTPP_UTIL_TIMESTAMP_INL_

namespace evpp {
    inline Timestamp::Timestamp()
        : us_(0)
    {
    }

    inline Timestamp Timestamp::Epoch() {
        return Timestamp();
    }

    inline bool Timestamp::IsEpoch() const {
        return us_ == 0;
    }

    inline Timestamp::Timestamp(int64_t us)
        : us_(us)
    {
    }

    inline Timestamp::Timestamp(const struct timeval& t)
        : us_(t.tv_sec * Duration::kMicrosecond + t.tv_usec)
    {
    }

    inline Timestamp Timestamp::Seconds(int64_t s) {
        return Timestamp(s * Duration::kMicrosecond);
    }
    inline Timestamp Timestamp::Milliseconds(int64_t ms) {
        return Timestamp(ms * Duration::kMillisecond);
    }
    inline Timestamp Timestamp::Microseconds(int64_t us) {
        return Timestamp(us);
    }

    inline Timestamp Timestamp::Now() {
        return Timestamp(utcmicrosecond());
    }

    inline int64_t Timestamp::ToSeconds() const {
        return us_ / Duration::kMicrosecond;
    }
    inline int64_t Timestamp::ToMilliseconds() const {
        return us_ / Duration::kMillisecond;
    }
    inline int64_t Timestamp::ToMicroseconds() const {
        return us_;
    }

    inline void Timestamp::To(struct timeval* t) const {
        t->tv_sec = (long)us_ / Duration::kMicrosecond;
        t->tv_usec = (long)us_ % Duration::kMicrosecond;
    }

    // Comparator
    inline bool Timestamp::operator< (const Timestamp& rhs) const {
        return us_ < rhs.us_;
    }
    inline bool Timestamp::operator==(const Timestamp& rhs) const {
        return us_ == rhs.us_;
    }

    // Math operator
    inline Timestamp Timestamp::operator+=(const Duration& rhs) {
        us_ += rhs.us_;
        return *this;
    }

    inline Timestamp Timestamp::operator+ (const Duration& rhs) const {
        Timestamp temp(*this);
        temp += rhs;
        return temp;
    }

    inline Timestamp Timestamp::operator-=(const Duration& rhs) {
        us_ -= rhs.us_;
        return *this;
    }

    inline Timestamp Timestamp::operator- (const Duration& rhs) const {
        Timestamp temp(*this);
        temp -= rhs;
        return temp;
    }

    inline Duration  Timestamp::operator- (const Timestamp& rhs) const {
        int64_t us = us_ - rhs.us_;
        return Duration(us);
    }

} // namespace evpp

#endif
