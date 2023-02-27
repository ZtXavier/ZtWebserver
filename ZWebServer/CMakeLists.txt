set(SRCS
	Channel.cpp
	Epoll.cpp
	EventLoop.cpp
	EventLoopThread.cpp
	EventLoopThreadPool.cpp
	Httpsolution.cpp
	server.cpp
	ThreadPool.cpp
	Timer.cpp
	Util.cpp
	Smain.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/base)
add_executable(ZWebServer ${SRCS})
target_link_libraries(ZWebServer libserver_base)

add_subdirectory(base)
add_subdirectory(tests)

