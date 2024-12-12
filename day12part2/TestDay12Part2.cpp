
#include "day12part2.h"

#include "common/logging.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"


class Day12Part2Test : public ::testing::Test {
protected:
    static void SetUpTestSuite() {

        SetLogLevel(LOGLEVEL_TRACE);

    }

    static void TearDownTestSuite() {}

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(Day12Part2Test, example1) {

    int totalPrice;

    Status status;
    status = computeTotalPrice("example.txt", &totalPrice);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(totalPrice, 80);
}


TEST_F(Day12Part2Test, example2) {

    int totalPrice;

    Status status;
    status = computeTotalPrice("example2.txt", &totalPrice);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(totalPrice, 436);
}


TEST_F(Day12Part2Test, example4) {

    int totalPrice;

    Status status;
    status = computeTotalPrice("example4.txt", &totalPrice);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(totalPrice, 236);
}


TEST_F(Day12Part2Test, example5) {

    int totalPrice;

    Status status;
    status = computeTotalPrice("example5.txt", &totalPrice);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(totalPrice, 368);
}


TEST_F(Day12Part2Test, example3) {

    int totalPrice;

    Status status;
    status = computeTotalPrice("example3.txt", &totalPrice);

    ASSERT_EQ(status, OK);

    EXPECT_EQ(totalPrice, 1206);
}
















