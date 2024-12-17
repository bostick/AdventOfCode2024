
#include "day16part2.h"

#include "common/logging.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"


class Day16Part2Test : public ::testing::Test {
protected:
    static void SetUpTestSuite() {

        SetLogLevel(LOGLEVEL_TRACE);

    }

    static void TearDownTestSuite() {}

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(Day16Part2Test, example1) {

    int bestPathTilesCount;

    Status status;
    status = computeBestPathTilesCount("example.txt", &bestPathTilesCount);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(bestPathTilesCount, 45);
}


TEST_F(Day16Part2Test, example2) {

    int bestPathTilesCount;

    Status status;
    status = computeBestPathTilesCount("example2.txt", &bestPathTilesCount);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(bestPathTilesCount, 64);
}
















