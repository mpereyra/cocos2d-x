# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.0.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.0.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64

# Include any dependencies generated for this target.
include tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/depend.make

# Include the progress variables for this target.
include tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/progress.make

# Include the compile flags for this target's objects.
include tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/flags.make

tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o: tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/flags.make
tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/tools/c-index-test/c-index-test.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/tools/c-index-test && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -std=gnu89 -o CMakeFiles/c-index-test.dir/c-index-test.c.o   -c /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/tools/c-index-test/c-index-test.c

tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/c-index-test.dir/c-index-test.c.i"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/tools/c-index-test && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -std=gnu89 -E /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/tools/c-index-test/c-index-test.c > CMakeFiles/c-index-test.dir/c-index-test.c.i

tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/c-index-test.dir/c-index-test.c.s"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/tools/c-index-test && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -std=gnu89 -S /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/tools/c-index-test/c-index-test.c -o CMakeFiles/c-index-test.dir/c-index-test.c.s

tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o.requires:
.PHONY : tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o.requires

tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o.provides: tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o.requires
	$(MAKE) -f tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/build.make tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o.provides.build
.PHONY : tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o.provides

tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o.provides.build: tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o

# Object files for target c-index-test
c__index__test_OBJECTS = \
"CMakeFiles/c-index-test.dir/c-index-test.c.o"

# External object files for target c-index-test
c__index__test_EXTERNAL_OBJECTS =

bin/c-index-test: tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o
bin/c-index-test: tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/build.make
bin/c-index-test: lib/libclang.3.7.dylib
bin/c-index-test: /usr/lib/libxml2.dylib
bin/c-index-test: tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../../bin/c-index-test"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/tools/c-index-test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/c-index-test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/build: bin/c-index-test
.PHONY : tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/build

tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/requires: tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/c-index-test.c.o.requires
.PHONY : tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/requires

tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/clean:
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/tools/c-index-test && $(CMAKE_COMMAND) -P CMakeFiles/c-index-test.dir/cmake_clean.cmake
.PHONY : tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/clean

tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/depend:
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/tools/c-index-test /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/tools/c-index-test /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/clang/tools/c-index-test/CMakeFiles/c-index-test.dir/depend

