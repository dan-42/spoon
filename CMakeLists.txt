cmake_minimum_required(VERSION 3.2)

### Hunter snapshot that will be used
include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/dan-42/hunter/archive/v0.18.55-beta.tar.gz"
        SHA1 "ccd0a029ef6649223624d6ce39d3016d8e08c6fc"        
)

project(spoon VERSION 0.0.1)
enable_testing()
add_subdirectory(src)

## excample
add_subdirectory(examples)

# Tests
#if(LIB_SPOON_UNIT_TESTS)
# add_subdirectory(test)
#endif()
