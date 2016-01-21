#ifndef LIBEVENTPP_UTIL_TIMESTAMP_INL_
#define LIBEVENTPP_UTIL_TIMESTAMP_INL_

namespace evpp {
    inline Timestamp::Timestamp()
        : d_(int64_t(0))
    {
    }

    inline Timestamp::Timestamp(int64_t nanoseconds)
        : d_(nanoseconds)
    {
    }

    inline bool Timestamp::IsEpoch() const {
        return d_.IsZero();
    }


    inline Timestamp::Timestamp(const struct timeval& t)
        : d_(t)
    {
    }

    inline Timestamp Timestamp::Now() {
        return Timestamp(Duration(int64_t(utcmicrosecond()*Duration::kMicrosecond)));
    }

    inline void Timestamp::To(struct timeval* t) const {
        d_.To(t);
    }
    
    inline struct timeval Timestamp::TimeVal() const {
        return d_.TimeVal();
    }

    // Comparator
    inline bool Timestamp::operator< (const Timestamp& rhs) const {
        return d_ < rhs.d_;
    }
    inline bool Timestamp::operator==(const Timestamp& rhs) const {
        return d_ == rhs.d_;
    }

    // Math operator
    inline Timestamp Timestamp::operator+=(const Duration& rhs) {
        d_ += rhs;
        return *this;
    }

    inline Timestamp Timestamp::operator+ (const Duration& rhs) const {
        Timestamp temp(*this);
        temp += rhs;
        return temp;
    }

    inline Timestamp Timestamp::operator-=(const Duration& rhs) {
        d_ -= rhs;
        return *this;
    }

    inline Timestamp Timestamp::operator- (const Duration& rhs) const {
        Timestamp temp(*this);
        temp -= rhs;
        return temp;
    }

    inline Duration  Timestamp::operator- (const Timestamp& rhs) const {
        int64_t ns = d_.Nanoseconds() - rhs.d_.Nanoseconds();
        return Duration(ns);
    }

} // namespace evpp

#endif
