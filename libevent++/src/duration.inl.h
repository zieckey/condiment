#ifndef LIBEVENTPP_UTIL_TIME_DURATION_INL_H_
#define LIBEVENTPP_UTIL_TIME_DURATION_INL_H_

namespace evpp {
    inline Duration::Duration()
        : us_(0)
    {
    }

    inline bool Duration::IsZero() const {
        return us_ == 0;
    }


    inline Duration::Duration(int64_t us)
        : us_(us)
    {
    }

    inline Duration::Duration(const struct timeval& t)
        : us_(t.tv_sec*kMicrosecond + t.tv_usec)
    {
    }

    // Factory methods from *seconds
    inline Duration Duration::Seconds(int64_t s) {
        return Duration(s * kMicrosecond);
    }
    inline Duration Duration::Milliseconds(int64_t ms) {
        return Duration(ms * kMillisecond);
    }
    inline Duration Duration::Microseconds(int64_t us) {
        return Duration(us);
    }

    // Property getters
    inline int64_t Duration::ToSeconds() const {
        return us_ / kMicrosecond;
    }
    inline int64_t Duration::ToMilliseconds() const {
        return us_ / kMillisecond;
    }
    inline int64_t Duration::ToMicroseconds() const {
        return us_;
    }
    inline int64_t Duration::ToMinutes() const {
        return ToSeconds() / kMunite;
    }
    // Converters
    inline void Duration::To(struct timeval* t) const {
        t->tv_sec = (long)us_ / kMicrosecond;
        t->tv_usec = (long)us_ % kMicrosecond;
    }

    // Comparator
    inline bool Duration::operator< (const Duration& rhs) const {
        return us_ < rhs.us_;
    }
    inline bool Duration::operator==(const Duration& rhs) const {
        return us_ == rhs.us_;
    }

    // Math operator
    inline Duration Duration::operator+=(const Duration& rhs) {
        us_ += rhs.us_;
        return *this;
    }
    inline Duration Duration::operator-=(const Duration& rhs) {
        us_ -= rhs.us_;
        return *this;
    }
    inline Duration Duration::operator*=(int n) {
        us_ *= n;
        return *this;
    }
    inline Duration Duration::operator/=(int n) {
        us_ /= n;
        return *this;
    }

} // namespace evpp
#endif // LIBEVENTPP_UTIL_TIME_DURATION_INL_H_
