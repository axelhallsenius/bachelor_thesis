#include "gtest/gtest.h"
extern "C" {
  // #include "test_map.c"
  // #include "test_draw.c"
  // #include "test_vessel.c"
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
