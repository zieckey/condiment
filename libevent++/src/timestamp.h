
#ifndef LIBEVENTPP_UTIL_TIMESTAMP_H_
#define LIBEVENTPP_UTIL_TIMESTAMP_H_

#include "duration.h"

namespace evpp {
    class Timestamp
    {
    public:
        Timestamp();
        Timestamp(int64_t nanoseconds);
        Timestamp(const struct timeval& t);

        static Timestamp Now(); // returns the current local time.

        struct timeval TimeVal() const;
        void To(struct timeval* t) const;

        // Unix returns t as a Unix time, the number of seconds elapsed
        // since January 1, 1970 UTC.
        int64_t Unix() const;

        // UnixNano returns t as a Unix time, the number of nanoseconds elapsed
        // since January 1, 1970 UTC. The result is undefined if the Unix time
        // in nanoseconds cannot be represented by an int64.
        int64_t UnixNano() const;

        void Add(Duration d);

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
        // us_ gives the number of nanoseconds elapsed since 
        // January 1, year 1 00:00:00 UTC. 
        // Approximately to year 2260.
        int64_t ns_;
    };
}

#include "timestamp.inl.h"

#endif

