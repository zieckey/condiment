#ifndef LIBEVENTPP_UTIL_TIME_DURATION_INL_H_
#define LIBEVENTPP_UTIL_TIME_DURATION_INL_H_

namespace evpp {
    inline Duration::Duration()
        : ns_(0)
    {}

    inline Duration::Duration(const struct timeval& t)
        : ns_(t.tv_sec*kSecond + t.tv_usec*kMicrosecond)
    {}

    inline Duration::Duration(int64_t nonaseconds)
        : ns_(nonaseconds)
    {}

    inline Duration::Duration(double seconds)
        : ns_((int64_t)(seconds*kSecond))
    {}

    inline bool Duration::IsZero() const {
        return ns_ == 0;
    }

    inline struct timeval Duration::TimeVal() const {
        struct timeval t;
        To(&t);
        return t;
    }

    inline void Duration::To(struct timeval* t) const {
        t->tv_sec = (long)(ns_ / kSecond);
        t->tv_usec = (long)(ns_ % kSecond) / kMicrosecond;
    }

    // Comparator
    inline bool Duration::operator< (const Duration& rhs) const {
        return ns_ < rhs.ns_;
    }
    inline bool Duration::operator==(const Duration& rhs) const {
        return ns_ == rhs.ns_;
    }

    // Math operator
    inline Duration Duration::operator+=(const Duration& rhs) {
        ns_ += rhs.ns_;
        return *this;
    }
    inline Duration Duration::operator-=(const Duration& rhs) {
        ns_ -= rhs.ns_;
        return *this;
    }
    inline Duration Duration::operator*=(int n) {
        ns_ *= n;
        return *this;
    }
    inline Duration Duration::operator/=(int n) {
        ns_ /= n;
        return *this;
    }

} // namespace evpp
#endif // LIBEVENTPP_UTIL_TIME_DURATION_INL_H_
