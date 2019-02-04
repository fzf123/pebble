cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -pthread -g -ggdb -O0 -Wall -Werror -D__GINI_DEBUG__")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -pthread -O3 -Wall -Werror")

file(GLOB_RECURSE QUEUE_SRC src/queue/*.cc)

SET(SRCS
    ${QUEUE_SRC}
    )
add_library(pebble STATIC ${SRCS})
