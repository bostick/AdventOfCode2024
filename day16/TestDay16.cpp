
#include "day16.h"

#include "common/logging.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"


class Day16Test : public ::testing::Test {
protected:
    static void SetUpTestSuite() {

        SetLogLevel(LOGLEVEL_TRACE);

    }

    static void TearDownTestSuite() {}

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(Day16Test, example1) {

    int lowestScore;

    Status status;
    status = computeLowestScore("example.txt", &lowestScore);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(lowestScore, 7036);
}


TEST_F(Day16Test, example2) {

    int lowestScore;

    Status status;
    status = computeLowestScore("example2.txt", &lowestScore);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(lowestScore, 11048);
}
















