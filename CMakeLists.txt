# 
# General
# 
cmake_minimum_required(VERSION 2.8)

project(arxlib)

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH}  ${arxlib_SOURCE_DIR}/config)

include_directories(${arxlib_SOURCE_DIR}/include)


# 
# Dependencies
# 
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
set(LIBRARIES ${LIBRARIES} ${Boost_LIBRARIES})

find_package(Qt4)
if(Qt4_FOUND)
  include_directories(${Qt_INCLUDE_DIRS})
  set(LIBRARIES ${LIBRARIES} ${Qt_LIBRARIES})
endif()

find_package(Vigra)
if(Vigra_FOUND)
  include_directories(${Vigra_INCLUDE_DIRS})
  set(LIBRARIES ${LIBRARIES} ${Vigra_LIBRARIES})
endif()

find_package(Magick++)
if(Magick++_FOUND)
  include_directories(${Magick++_INCLUDE_DIRS})
  set(LIBRARIES ${LIBRARIES} ${Magick++_LIBRARIES})
endif()


# 
# Tests
# 
enable_testing()

add_executable(Foreach ${arxlib_SOURCE_DIR}/test/arx/Foreach.cpp)
target_link_libraries(Foreach ${LIBRARIES})
add_test(Foreach Foreach)

add_executable(HasXxx ${arxlib_SOURCE_DIR}/test/arx/HasXxx.cpp)
target_link_libraries(HasXxx ${LIBRARIES})
add_test(HasXxx HasXxx)































