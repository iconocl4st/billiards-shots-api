cmake_minimum_required(VERSION 3.13)
project(shots_api)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE on)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE HINTS /usr/share/cmake/Modules/)



message(STATUS "The directory: $ENV{REPOS}")


add_executable(
        shots_api

		${CMAKE_CURRENT_SOURCE_DIR}/src/shots_api.cpp
)

target_include_directories(
	shots_api

	PUBLIC
	${PROJECT_SOURCE_DIR}/src

	${PROJECT_SOURCE_DIR}/../billiards-common/src
	$ENV{REPOS}/billiards-common/src
)


target_link_libraries(
	shots_api

	Threads::Threads
	${Boost_SYSTEM_LIBRARY}
	Eigen3::Eigen
)


#add_executable(
#		test_shots
#
#		${CMAKE_CURRENT_SOURCE_DIR}/src/shots_api.cpp
#)
#
#target_include_directories(
#		shots_api
#
#		PUBLIC
#		$ENV{REPOS}/billiards-billiards_common/src
#)
#
#
#target_link_libraries(
#		shots_api
#
#		Threads::Threads
#		${Boost_SYSTEM_LIBRARY}
#)


install(
	TARGETS
		shots_api
	DESTINATION
		/app
	PERMISSIONS
		OWNER_WRITE OWNER_READ OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE
)
