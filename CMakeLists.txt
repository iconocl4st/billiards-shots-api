cmake_minimum_required(VERSION 3.13)
project(shots_api)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE on)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

add_executable(
        shots_api

		${CMAKE_CURRENT_SOURCE_DIR}/src/shots_api.cpp
)

target_include_directories(
	shots_api
	
	PUBLIC
	$ENV{REPOS}/billiards-common/src
)


target_link_libraries(
	shots_api

	Threads::Threads
	${Boost_SYSTEM_LIBRARY}
)


install(TARGETS shots_api DESTINATION /app)
