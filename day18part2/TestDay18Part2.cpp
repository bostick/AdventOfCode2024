
#include "day18part2.h"

#include "common/logging.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"


class Day18Part2Test : public ::testing::Test {
protected:
    static void SetUpTestSuite() {

        SetLogLevel(LOGLEVEL_TRACE);

    }

    static void TearDownTestSuite() {}

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(Day18Part2Test, example1) {

    Coord firstBadCoord;

    Status status;
    status = computeFirstBadCoord("example.txt", &firstBadCoord);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(firstBadCoord, (Coord{ 1, 6 }) );
}
















