cmake_minimum_required(VERSION 2.8)

project(ZtWebServerLearning ZtWS)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE"Debug")
endif()

set(ZtWS_FLAGS
	-g
	-wall
	-std=c++11
	-D_PTHREADS
	-Wno-unused-parameter
)

set(CMAKE_ZtWS_COMPILER "g++")
set(CMAKE_ZtWS_FLAGS_DEBUG "-O0")
set(CMAKE_ZtWS_FLAGS_RELEASE "-O0")

string(REPLACE ";"" "CMAKE_ZtWS_FLAGS "${ZtWS_FLAGS}")

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)

message(STATUS"ZtWS_FLAGS=" ${CMAKE_ZtWS_FLAGS} " "${CMAKE_ZtWS_FLAGS_${BUILD_TYPE}})

add_subdirectory(ZtWebServerLearing)
