
#include "src/exp.h"
#include "./test_common.h"
#include "src/duration.h"
#include "src/timestamp.h"

TEST_UNIT(Duration_Compare)
{
    evpp::Duration d0(0);
    evpp::Duration d1(1);
    evpp::Duration d2(2);
    evpp::Duration d3(2);
    H_TEST_ASSERT(d0 < d1);
    H_TEST_ASSERT(d1 < d2);
    H_TEST_ASSERT(d2 == d3);
    H_TEST_ASSERT(d0.IsZero());
}

TEST_UNIT(Timestamp)
{
}

