# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build

# Include any dependencies generated for this target.
include src/libmongoc/CMakeFiles/example-start-at-optime.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/libmongoc/CMakeFiles/example-start-at-optime.dir/compiler_depend.make

# Include the progress variables for this target.
include src/libmongoc/CMakeFiles/example-start-at-optime.dir/progress.make

# Include the compile flags for this target's objects.
include src/libmongoc/CMakeFiles/example-start-at-optime.dir/flags.make

src/libmongoc/CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.o: src/libmongoc/CMakeFiles/example-start-at-optime.dir/flags.make
src/libmongoc/CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.o: /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/src/libmongoc/examples/example-start-at-optime.c
src/libmongoc/CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.o: src/libmongoc/CMakeFiles/example-start-at-optime.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/libmongoc/CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.o"
	cd /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build/src/libmongoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/libmongoc/CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.o -MF CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.o.d -o CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.o -c /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/src/libmongoc/examples/example-start-at-optime.c

src/libmongoc/CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.i"
	cd /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build/src/libmongoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/src/libmongoc/examples/example-start-at-optime.c > CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.i

src/libmongoc/CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.s"
	cd /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build/src/libmongoc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/src/libmongoc/examples/example-start-at-optime.c -o CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.s

# Object files for target example-start-at-optime
example__start__at__optime_OBJECTS = \
"CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.o"

# External object files for target example-start-at-optime
example__start__at__optime_EXTERNAL_OBJECTS =

src/libmongoc/example-start-at-optime: src/libmongoc/CMakeFiles/example-start-at-optime.dir/examples/example-start-at-optime.c.o
src/libmongoc/example-start-at-optime: src/libmongoc/CMakeFiles/example-start-at-optime.dir/build.make
src/libmongoc/example-start-at-optime: src/libmongoc/libmongoc-1.0.so.0.0.0
src/libmongoc/example-start-at-optime: /usr/lib/aarch64-linux-gnu/libsasl2.so
src/libmongoc/example-start-at-optime: /usr/lib/aarch64-linux-gnu/libssl.so
src/libmongoc/example-start-at-optime: /usr/lib/aarch64-linux-gnu/libcrypto.so
src/libmongoc/example-start-at-optime: /usr/lib/aarch64-linux-gnu/libsnappy.so
src/libmongoc/example-start-at-optime: /usr/lib/aarch64-linux-gnu/libz.so
src/libmongoc/example-start-at-optime: /usr/lib/aarch64-linux-gnu/libzstd.so
src/libmongoc/example-start-at-optime: /usr/lib/aarch64-linux-gnu/libicuuc.so
src/libmongoc/example-start-at-optime: /usr/lib/aarch64-linux-gnu/libmongocrypt.so
src/libmongoc/example-start-at-optime: src/libbson/libbson-1.0.so.0.0.0
src/libmongoc/example-start-at-optime: src/libmongoc/CMakeFiles/example-start-at-optime.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable example-start-at-optime"
	cd /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build/src/libmongoc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/example-start-at-optime.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/libmongoc/CMakeFiles/example-start-at-optime.dir/build: src/libmongoc/example-start-at-optime
.PHONY : src/libmongoc/CMakeFiles/example-start-at-optime.dir/build

src/libmongoc/CMakeFiles/example-start-at-optime.dir/clean:
	cd /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build/src/libmongoc && $(CMAKE_COMMAND) -P CMakeFiles/example-start-at-optime.dir/cmake_clean.cmake
.PHONY : src/libmongoc/CMakeFiles/example-start-at-optime.dir/clean

src/libmongoc/CMakeFiles/example-start-at-optime.dir/depend:
	cd /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4 /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/src/libmongoc /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build/src/libmongoc /home/usuario/operating_systems/FS_DB_Project/mongo-c-driver-1.17.4/cmake-build/src/libmongoc/CMakeFiles/example-start-at-optime.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/libmongoc/CMakeFiles/example-start-at-optime.dir/depend

