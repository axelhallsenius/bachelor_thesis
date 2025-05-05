#include "gtest/gtest.h"
#include "test_map.cpp"
#include "test_vessel.cpp"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
