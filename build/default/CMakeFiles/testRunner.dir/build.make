# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pjoter/eclipse-workspace/economy2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pjoter/eclipse-workspace/economy2/build/default

# Include any dependencies generated for this target.
include CMakeFiles/testRunner.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testRunner.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testRunner.dir/flags.make

CMakeFiles/testRunner.dir/test.cpp.o: CMakeFiles/testRunner.dir/flags.make
CMakeFiles/testRunner.dir/test.cpp.o: ../../test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pjoter/eclipse-workspace/economy2/build/default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testRunner.dir/test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testRunner.dir/test.cpp.o -c /home/pjoter/eclipse-workspace/economy2/test.cpp

CMakeFiles/testRunner.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testRunner.dir/test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pjoter/eclipse-workspace/economy2/test.cpp > CMakeFiles/testRunner.dir/test.cpp.i

CMakeFiles/testRunner.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testRunner.dir/test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pjoter/eclipse-workspace/economy2/test.cpp -o CMakeFiles/testRunner.dir/test.cpp.s

# Object files for target testRunner
testRunner_OBJECTS = \
"CMakeFiles/testRunner.dir/test.cpp.o"

# External object files for target testRunner
testRunner_EXTERNAL_OBJECTS =

testRunner: CMakeFiles/testRunner.dir/test.cpp.o
testRunner: CMakeFiles/testRunner.dir/build.make
testRunner: concurrency/libconcurrency.a
testRunner: banking/libbanking.a
testRunner: lib/libgtest_main.a
testRunner: lib/libgmock_main.a
testRunner: lib/libgmock.a
testRunner: lib/libgtest.a
testRunner: CMakeFiles/testRunner.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pjoter/eclipse-workspace/economy2/build/default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable testRunner"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testRunner.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testRunner.dir/build: testRunner

.PHONY : CMakeFiles/testRunner.dir/build

CMakeFiles/testRunner.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testRunner.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testRunner.dir/clean

CMakeFiles/testRunner.dir/depend:
	cd /home/pjoter/eclipse-workspace/economy2/build/default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pjoter/eclipse-workspace/economy2 /home/pjoter/eclipse-workspace/economy2 /home/pjoter/eclipse-workspace/economy2/build/default /home/pjoter/eclipse-workspace/economy2/build/default /home/pjoter/eclipse-workspace/economy2/build/default/CMakeFiles/testRunner.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testRunner.dir/depend
