#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

namespace
{
    /* Just one test to create a main. All other tests are in their separate source files. */
    TEST_CASE("Main test case", "[main]")
    {
    }
}; // namespace