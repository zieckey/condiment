
#ifndef LIBEVENTPP_UTIL_TIMESTAMP_H_
#define LIBEVENTPP_UTIL_TIMESTAMP_H_

#include <boost/operators.hpp>

#include "duration.h"

namespace evpp {
    class Timestamp : boost::less_than_comparable<Timestamp>,
        boost::equality_comparable<Timestamp>
    {
    public:
        // Constructor and factory to get the epoch timestamp
        Timestamp();
        static Timestamp Epoch();

        // Constructors from timeval
        Timestamp(const struct timeval& t);

        // Factory methods from *seconds
        static Timestamp Seconds(int64_t s);
        static Timestamp Milliseconds(int64_t ms);
        static Timestamp Microseconds(int64_t us);

        // Factory methods from current time
        static Timestamp Now();

        // Property getters
        int64_t ToSeconds() const;
        int64_t ToMilliseconds() const;
        int64_t ToMicroseconds() const;
        int64_t ToNanoseconds() const;

        // Converters
        void To(struct timeval* t) const;

        // Comparator
        bool IsEpoch() const;
        bool operator< (const Timestamp& rhs) const;
        bool operator==(const Timestamp& rhs) const;

        // Math operator
        Timestamp operator+=(const Duration& rhs);
        Timestamp operator+ (const Duration& rhs) const;
        Timestamp operator-=(const Duration& rhs);
        Timestamp operator- (const Duration& rhs) const;
        Duration operator- (const Timestamp& rhs) const;

    private:
        explicit Timestamp(int64_t us);
        int64_t us_;
    };
}

#include "timestamp.inl.h"

#endif

