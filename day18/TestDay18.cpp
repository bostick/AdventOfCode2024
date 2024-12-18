
#include "day18.h"

#include "common/logging.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"


class Day18Test : public ::testing::Test {
protected:
    static void SetUpTestSuite() {

        SetLogLevel(LOGLEVEL_TRACE);

    }

    static void TearDownTestSuite() {}

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(Day18Test, example1) {

    int lowestStepCount;

    Status status;
    status = computeLowestStepCount("example.txt", &lowestStepCount);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(lowestStepCount, 22);
}
















