#include "log.hh"

#include <gtest/gtest.h>

using namespace wt;

int main(int argc, char** argv) {
    setup_logging();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
