# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zt/program/XZtwebserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zt/program/XZtwebserver

# Include any dependencies generated for this target.
include ZWebServer/base/CMakeFiles/libserver_base.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include ZWebServer/base/CMakeFiles/libserver_base.dir/compiler_depend.make

# Include the progress variables for this target.
include ZWebServer/base/CMakeFiles/libserver_base.dir/progress.make

# Include the compile flags for this target's objects.
include ZWebServer/base/CMakeFiles/libserver_base.dir/flags.make

ZWebServer/base/CMakeFiles/libserver_base.dir/Asynclogging.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/flags.make
ZWebServer/base/CMakeFiles/libserver_base.dir/Asynclogging.cpp.o: ZWebServer/base/Asynclogging.cpp
ZWebServer/base/CMakeFiles/libserver_base.dir/Asynclogging.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zt/program/XZtwebserver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ZWebServer/base/CMakeFiles/libserver_base.dir/Asynclogging.cpp.o"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ZWebServer/base/CMakeFiles/libserver_base.dir/Asynclogging.cpp.o -MF CMakeFiles/libserver_base.dir/Asynclogging.cpp.o.d -o CMakeFiles/libserver_base.dir/Asynclogging.cpp.o -c /home/zt/program/XZtwebserver/ZWebServer/base/Asynclogging.cpp

ZWebServer/base/CMakeFiles/libserver_base.dir/Asynclogging.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libserver_base.dir/Asynclogging.cpp.i"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zt/program/XZtwebserver/ZWebServer/base/Asynclogging.cpp > CMakeFiles/libserver_base.dir/Asynclogging.cpp.i

ZWebServer/base/CMakeFiles/libserver_base.dir/Asynclogging.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libserver_base.dir/Asynclogging.cpp.s"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zt/program/XZtwebserver/ZWebServer/base/Asynclogging.cpp -o CMakeFiles/libserver_base.dir/Asynclogging.cpp.s

ZWebServer/base/CMakeFiles/libserver_base.dir/Fileappend.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/flags.make
ZWebServer/base/CMakeFiles/libserver_base.dir/Fileappend.cpp.o: ZWebServer/base/Fileappend.cpp
ZWebServer/base/CMakeFiles/libserver_base.dir/Fileappend.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zt/program/XZtwebserver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object ZWebServer/base/CMakeFiles/libserver_base.dir/Fileappend.cpp.o"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ZWebServer/base/CMakeFiles/libserver_base.dir/Fileappend.cpp.o -MF CMakeFiles/libserver_base.dir/Fileappend.cpp.o.d -o CMakeFiles/libserver_base.dir/Fileappend.cpp.o -c /home/zt/program/XZtwebserver/ZWebServer/base/Fileappend.cpp

ZWebServer/base/CMakeFiles/libserver_base.dir/Fileappend.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libserver_base.dir/Fileappend.cpp.i"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zt/program/XZtwebserver/ZWebServer/base/Fileappend.cpp > CMakeFiles/libserver_base.dir/Fileappend.cpp.i

ZWebServer/base/CMakeFiles/libserver_base.dir/Fileappend.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libserver_base.dir/Fileappend.cpp.s"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zt/program/XZtwebserver/ZWebServer/base/Fileappend.cpp -o CMakeFiles/libserver_base.dir/Fileappend.cpp.s

ZWebServer/base/CMakeFiles/libserver_base.dir/LoggFile.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/flags.make
ZWebServer/base/CMakeFiles/libserver_base.dir/LoggFile.cpp.o: ZWebServer/base/LoggFile.cpp
ZWebServer/base/CMakeFiles/libserver_base.dir/LoggFile.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zt/program/XZtwebserver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object ZWebServer/base/CMakeFiles/libserver_base.dir/LoggFile.cpp.o"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ZWebServer/base/CMakeFiles/libserver_base.dir/LoggFile.cpp.o -MF CMakeFiles/libserver_base.dir/LoggFile.cpp.o.d -o CMakeFiles/libserver_base.dir/LoggFile.cpp.o -c /home/zt/program/XZtwebserver/ZWebServer/base/LoggFile.cpp

ZWebServer/base/CMakeFiles/libserver_base.dir/LoggFile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libserver_base.dir/LoggFile.cpp.i"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zt/program/XZtwebserver/ZWebServer/base/LoggFile.cpp > CMakeFiles/libserver_base.dir/LoggFile.cpp.i

ZWebServer/base/CMakeFiles/libserver_base.dir/LoggFile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libserver_base.dir/LoggFile.cpp.s"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zt/program/XZtwebserver/ZWebServer/base/LoggFile.cpp -o CMakeFiles/libserver_base.dir/LoggFile.cpp.s

ZWebServer/base/CMakeFiles/libserver_base.dir/Logging.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/flags.make
ZWebServer/base/CMakeFiles/libserver_base.dir/Logging.cpp.o: ZWebServer/base/Logging.cpp
ZWebServer/base/CMakeFiles/libserver_base.dir/Logging.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zt/program/XZtwebserver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object ZWebServer/base/CMakeFiles/libserver_base.dir/Logging.cpp.o"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ZWebServer/base/CMakeFiles/libserver_base.dir/Logging.cpp.o -MF CMakeFiles/libserver_base.dir/Logging.cpp.o.d -o CMakeFiles/libserver_base.dir/Logging.cpp.o -c /home/zt/program/XZtwebserver/ZWebServer/base/Logging.cpp

ZWebServer/base/CMakeFiles/libserver_base.dir/Logging.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libserver_base.dir/Logging.cpp.i"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zt/program/XZtwebserver/ZWebServer/base/Logging.cpp > CMakeFiles/libserver_base.dir/Logging.cpp.i

ZWebServer/base/CMakeFiles/libserver_base.dir/Logging.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libserver_base.dir/Logging.cpp.s"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zt/program/XZtwebserver/ZWebServer/base/Logging.cpp -o CMakeFiles/libserver_base.dir/Logging.cpp.s

ZWebServer/base/CMakeFiles/libserver_base.dir/Thread.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/flags.make
ZWebServer/base/CMakeFiles/libserver_base.dir/Thread.cpp.o: ZWebServer/base/Thread.cpp
ZWebServer/base/CMakeFiles/libserver_base.dir/Thread.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zt/program/XZtwebserver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object ZWebServer/base/CMakeFiles/libserver_base.dir/Thread.cpp.o"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ZWebServer/base/CMakeFiles/libserver_base.dir/Thread.cpp.o -MF CMakeFiles/libserver_base.dir/Thread.cpp.o.d -o CMakeFiles/libserver_base.dir/Thread.cpp.o -c /home/zt/program/XZtwebserver/ZWebServer/base/Thread.cpp

ZWebServer/base/CMakeFiles/libserver_base.dir/Thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libserver_base.dir/Thread.cpp.i"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zt/program/XZtwebserver/ZWebServer/base/Thread.cpp > CMakeFiles/libserver_base.dir/Thread.cpp.i

ZWebServer/base/CMakeFiles/libserver_base.dir/Thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libserver_base.dir/Thread.cpp.s"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zt/program/XZtwebserver/ZWebServer/base/Thread.cpp -o CMakeFiles/libserver_base.dir/Thread.cpp.s

ZWebServer/base/CMakeFiles/libserver_base.dir/Logstream.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/flags.make
ZWebServer/base/CMakeFiles/libserver_base.dir/Logstream.cpp.o: ZWebServer/base/Logstream.cpp
ZWebServer/base/CMakeFiles/libserver_base.dir/Logstream.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zt/program/XZtwebserver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object ZWebServer/base/CMakeFiles/libserver_base.dir/Logstream.cpp.o"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ZWebServer/base/CMakeFiles/libserver_base.dir/Logstream.cpp.o -MF CMakeFiles/libserver_base.dir/Logstream.cpp.o.d -o CMakeFiles/libserver_base.dir/Logstream.cpp.o -c /home/zt/program/XZtwebserver/ZWebServer/base/Logstream.cpp

ZWebServer/base/CMakeFiles/libserver_base.dir/Logstream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libserver_base.dir/Logstream.cpp.i"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zt/program/XZtwebserver/ZWebServer/base/Logstream.cpp > CMakeFiles/libserver_base.dir/Logstream.cpp.i

ZWebServer/base/CMakeFiles/libserver_base.dir/Logstream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libserver_base.dir/Logstream.cpp.s"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zt/program/XZtwebserver/ZWebServer/base/Logstream.cpp -o CMakeFiles/libserver_base.dir/Logstream.cpp.s

ZWebServer/base/CMakeFiles/libserver_base.dir/CountDownLatch.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/flags.make
ZWebServer/base/CMakeFiles/libserver_base.dir/CountDownLatch.cpp.o: ZWebServer/base/CountDownLatch.cpp
ZWebServer/base/CMakeFiles/libserver_base.dir/CountDownLatch.cpp.o: ZWebServer/base/CMakeFiles/libserver_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zt/program/XZtwebserver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object ZWebServer/base/CMakeFiles/libserver_base.dir/CountDownLatch.cpp.o"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ZWebServer/base/CMakeFiles/libserver_base.dir/CountDownLatch.cpp.o -MF CMakeFiles/libserver_base.dir/CountDownLatch.cpp.o.d -o CMakeFiles/libserver_base.dir/CountDownLatch.cpp.o -c /home/zt/program/XZtwebserver/ZWebServer/base/CountDownLatch.cpp

ZWebServer/base/CMakeFiles/libserver_base.dir/CountDownLatch.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libserver_base.dir/CountDownLatch.cpp.i"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zt/program/XZtwebserver/ZWebServer/base/CountDownLatch.cpp > CMakeFiles/libserver_base.dir/CountDownLatch.cpp.i

ZWebServer/base/CMakeFiles/libserver_base.dir/CountDownLatch.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libserver_base.dir/CountDownLatch.cpp.s"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zt/program/XZtwebserver/ZWebServer/base/CountDownLatch.cpp -o CMakeFiles/libserver_base.dir/CountDownLatch.cpp.s

# Object files for target libserver_base
libserver_base_OBJECTS = \
"CMakeFiles/libserver_base.dir/Asynclogging.cpp.o" \
"CMakeFiles/libserver_base.dir/Fileappend.cpp.o" \
"CMakeFiles/libserver_base.dir/LoggFile.cpp.o" \
"CMakeFiles/libserver_base.dir/Logging.cpp.o" \
"CMakeFiles/libserver_base.dir/Thread.cpp.o" \
"CMakeFiles/libserver_base.dir/Logstream.cpp.o" \
"CMakeFiles/libserver_base.dir/CountDownLatch.cpp.o"

# External object files for target libserver_base
libserver_base_EXTERNAL_OBJECTS =

ZWebServer/base/libserver_base.a: ZWebServer/base/CMakeFiles/libserver_base.dir/Asynclogging.cpp.o
ZWebServer/base/libserver_base.a: ZWebServer/base/CMakeFiles/libserver_base.dir/Fileappend.cpp.o
ZWebServer/base/libserver_base.a: ZWebServer/base/CMakeFiles/libserver_base.dir/LoggFile.cpp.o
ZWebServer/base/libserver_base.a: ZWebServer/base/CMakeFiles/libserver_base.dir/Logging.cpp.o
ZWebServer/base/libserver_base.a: ZWebServer/base/CMakeFiles/libserver_base.dir/Thread.cpp.o
ZWebServer/base/libserver_base.a: ZWebServer/base/CMakeFiles/libserver_base.dir/Logstream.cpp.o
ZWebServer/base/libserver_base.a: ZWebServer/base/CMakeFiles/libserver_base.dir/CountDownLatch.cpp.o
ZWebServer/base/libserver_base.a: ZWebServer/base/CMakeFiles/libserver_base.dir/build.make
ZWebServer/base/libserver_base.a: ZWebServer/base/CMakeFiles/libserver_base.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zt/program/XZtwebserver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library libserver_base.a"
	cd /home/zt/program/XZtwebserver/ZWebServer/base && $(CMAKE_COMMAND) -P CMakeFiles/libserver_base.dir/cmake_clean_target.cmake
	cd /home/zt/program/XZtwebserver/ZWebServer/base && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libserver_base.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ZWebServer/base/CMakeFiles/libserver_base.dir/build: ZWebServer/base/libserver_base.a
.PHONY : ZWebServer/base/CMakeFiles/libserver_base.dir/build

ZWebServer/base/CMakeFiles/libserver_base.dir/clean:
	cd /home/zt/program/XZtwebserver/ZWebServer/base && $(CMAKE_COMMAND) -P CMakeFiles/libserver_base.dir/cmake_clean.cmake
.PHONY : ZWebServer/base/CMakeFiles/libserver_base.dir/clean

ZWebServer/base/CMakeFiles/libserver_base.dir/depend:
	cd /home/zt/program/XZtwebserver && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zt/program/XZtwebserver /home/zt/program/XZtwebserver/ZWebServer/base /home/zt/program/XZtwebserver /home/zt/program/XZtwebserver/ZWebServer/base /home/zt/program/XZtwebserver/ZWebServer/base/CMakeFiles/libserver_base.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ZWebServer/base/CMakeFiles/libserver_base.dir/depend

