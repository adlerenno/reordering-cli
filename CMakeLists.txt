cmake_minimum_required(VERSION 3.10)
project(reordering VERSION 1.0.0 DESCRIPTION "Reordering Paper with CLI.")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17) # At least 17 for parallel support.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Error on undefined references
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl")
else()
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
endif()

# Allow support for large files
add_definitions(-D_LARGEFILE_SOURCE -D_LARGE_FILES -D_FILE_OFFSET_BITS=64)

# get files with "find include src -type f -name '*.c' | sort"
set(SOURCES
        compress/compressUnordering.hpp
        compress/compressUnordering.cpp
        compress/reorderingHyperedges.hpp
        compress/reorderingHyperedges.cpp
        compress/reorderingVertices.hpp
        compress/reorderingVertices.cpp
        compress/reorderingVerticesHyperedges.hpp
        compress/reorderingVerticesHyperedges.cpp
        main.cpp
        get_graph.cpp
        get_graph.h
        read/NQAdjList.hpp
        read/NQAdjList.cpp
        read/NQOneDimArray.hpp
        read/NQOneDimArray.cpp
        read/query.cpp
        read/query.hpp
)

set(INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        /usr/local/include
        compress
        read
)


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_directories(${PROJECT_NAME} PRIVATE "/usr/local/lib")  # necessary on MAC to find libraries.

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${CMAKE_CURRENT_BINARY_DIR}/cgraph.h)

find_package(OpenMP QUIET)

if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP found. Enabling parallel support.")
  target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
  target_compile_definitions(${PROJECT_NAME} PRIVATE USE_OPENMP=1)
else()
  message(WARNING "OpenMP not found. Building without parallel support.")
endif()
