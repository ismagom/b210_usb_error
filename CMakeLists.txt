project(b2xx_usb_error)

cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

FIND_PACKAGE(UHD REQUIRED)

set(BOOST_REQUIRED_COMPONENTS
  program_options
  system
)

find_package(Boost "1.35" COMPONENTS ${BOOST_REQUIRED_COMPONENTS})

include_directories(${UHD_INCLUDE_DIRS})

add_executable(change_srate change_srate.cpp)
target_link_libraries(change_srate ${UHD_LIBRARIES} ${Boost_LIBRARIES})

add_executable(change_freq change_freq.cpp)
target_link_libraries(change_freq ${UHD_LIBRARIES} ${Boost_LIBRARIES})

add_executable(start_stop start_stop.cpp)
target_link_libraries(start_stop ${UHD_LIBRARIES} ${Boost_LIBRARIES})

