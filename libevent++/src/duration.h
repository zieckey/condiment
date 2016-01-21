#ifndef LIBEVENTPP_UTIL_TIME_DURATION_H_
#define LIBEVENTPP_UTIL_TIME_DURATION_H_

#include <boost/operators.hpp>

#include "inner_pre.h"
#include "gettimeofday.h"

namespace evpp {

    // A Duration represents the elapsed time between two instants
    // as an int64 nanosecond count.  The representation limits the
    // largest representable duration to approximately 290 years.
    class Duration
    {
    public:
        enum Unit {
            kNanosecond = 1L,
            kMicrosecond = 1000 * kNanosecond,
            kMillisecond = 1000 * kMicrosecond,
            kSecond = 1000 * kMillisecond,
            kMinute = 60 * kSecond,
            kHour = 60 * kMinute,
        };
    public:
        Duration();
        Duration(const struct timeval& t);
        Duration(int64_t nonasecond);
        Duration(double second);

        int64_t Nanoseconds() const;
        double Seconds() const;
        double Milliseconds() const;
        double Microseconds() const;
        double Minutes() const;
        double Hours() const;

        struct timeval TimeVal() const;
        void To(struct timeval* t) const;

        bool IsZero() const;
        bool operator< (const Duration& rhs) const;
        bool operator==(const Duration& rhs) const;

        // Math operator
        Duration operator+=(const Duration& rhs);
        Duration operator-=(const Duration& rhs);
        Duration operator*=(int n);
        Duration operator/=(int n);

    private:
        int64_t ns_; // nanoseconds
    };
} // namespace evpp

#include "duration.inl.h"

#endif // LDD_UTIL_TIME_DIFF_H_
