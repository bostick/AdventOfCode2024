
#include "day12.h"

#include "common/logging.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"


class Day12Test : public ::testing::Test {
protected:
    static void SetUpTestSuite() {

        SetLogLevel(LOGLEVEL_TRACE);

    }

    static void TearDownTestSuite() {}

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(Day12Test, example1) {

    int totalPrice;

    Status status;
    status = computeTotalPrice("example.txt", &totalPrice);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(totalPrice, 140);
}


TEST_F(Day12Test, example2) {

    int totalPrice;

    Status status;
    status = computeTotalPrice("example2.txt", &totalPrice);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(totalPrice, 772);
}


TEST_F(Day12Test, example3) {

    int totalPrice;

    Status status;
    status = computeTotalPrice("example3.txt", &totalPrice);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(totalPrice, 1930);
}
















