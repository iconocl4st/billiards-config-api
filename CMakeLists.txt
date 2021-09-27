cmake_minimum_required(VERSION 3.13)
project(config_api)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE on)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

add_executable(
        config_api

		${CMAKE_CURRENT_SOURCE_DIR}/src/config_api.cpp
)

target_include_directories(
	config_api
	
	PUBLIC
	$ENV{REPOS}/billiards-common/src
)


target_link_libraries(
    	config_api

	    Threads::Threads
		${Boost_SYSTEM_LIBRARY}
)

install(TARGETS config_api DESTINATION /app)
