#ifndef LIBEVENTPP_UTIL_TIME_DURATION_H_
#define LIBEVENTPP_UTIL_TIME_DURATION_H_

#include <boost/operators.hpp>

#include "inner_pre.h"
#include "gettimeofday.h"

namespace evpp {
    class Duration : boost::less_than_comparable<Duration>,
        boost::equality_comparable<Duration>,
        boost::addable<Duration>,
        boost::subtractable<Duration>,
        boost::multipliable<Duration, int>,
        boost::dividable<Duration, int>
    {
        friend class Timestamp;
    public:
        enum Unit {
            kMunite = 60LL,
            kSecond = 1LL,
            kMillisecond = 1000LL,
            kMicrosecond = 1000000LL,
            kNanosecond = 1000000000LL,
        };

    public:
        Duration();

        // Constructors from timeval
        Duration(const struct timeval& t);

        // Factory methods from *seconds
        static Duration Seconds(int64_t s);
        static Duration Milliseconds(int64_t ms);
        static Duration Microseconds(int64_t us);

        // Property getters
        int64_t ToMinutes() const;
        int64_t ToSeconds() const;
        int64_t ToMilliseconds() const;
        int64_t ToMicroseconds() const;

        // Converters
        void To(struct timeval* t) const;

        // Comparator
        bool IsZero() const;
        bool operator< (const Duration& rhs) const;
        bool operator==(const Duration& rhs) const;

        // Math operator
        Duration operator+=(const Duration& rhs);
        Duration operator-=(const Duration& rhs);
        Duration operator*=(int n);
        Duration operator/=(int n);

    private:
        explicit Duration(int64_t us);
        int64_t us_;
    };
} // namespace evpp

#include "duration.inl.h"

#endif // LDD_UTIL_TIME_DIFF_H_
