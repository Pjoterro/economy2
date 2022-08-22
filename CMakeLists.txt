cmake_minimum_required (VERSION 3.13.0)

project (economy2 VERSION 0.9.0)

add_subdirectory(banking)


include (FetchContent)
FetchContent_Declare(googletest
	GIT_REPOSITORY https://github.com/google/googletest
	GIT_TAG release-1.12.1)
FetchCOntent_GetProperties(googletest)
#googletest_POPULATED
#googletest_SOURCE_DIR
#googletest_BUILD_DIR
if(NOT googltest_POPULATED)
	FetchContent_Populate(googletest)
	add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()

set(Boost_NO_BOOST_CMAKE ON)

add_definitions(-DBOOST_LOG_DYN_LINK)
FIND_PACKAGE(Boost COMPONENTS log REQUIRED)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g -lboost_log_setup")

add_executable (
	economy2
	economy2.cpp
	)

add_executable (
	economy2test
	test/bankingTest.cpp
	)

target_link_libraries(economy2 PRIVATE banking gtest_main Boost::log_setup Boost::log)

target_link_libraries(economy2test banking gtest gmock gtest_main Boost::log_setup Boost::log)