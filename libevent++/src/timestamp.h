
#ifndef LIBEVENTPP_UTIL_TIMESTAMP_H_
#define LIBEVENTPP_UTIL_TIMESTAMP_H_

#include "duration.h"

namespace evpp {
    class Timestamp
    {
    public:
        Timestamp();
        static Timestamp Now();

        Timestamp(int64_t nanoseconds);
        Timestamp(const Duration& d);
        Timestamp(const struct timeval& t);

        struct timeval TimeVal() const;
        void To(struct timeval* t) const;

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
        Duration d_;
    };
}

#include "timestamp.inl.h"

#endif

