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
include tools/llvm-size/CMakeFiles/llvm-size.dir/depend.make

# Include the progress variables for this target.
include tools/llvm-size/CMakeFiles/llvm-size.dir/progress.make

# Include the compile flags for this target's objects.
include tools/llvm-size/CMakeFiles/llvm-size.dir/flags.make

tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o: tools/llvm-size/CMakeFiles/llvm-size.dir/flags.make
tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/llvm-size/llvm-size.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-size && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/llvm-size.dir/llvm-size.cpp.o -c /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/llvm-size/llvm-size.cpp

tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/llvm-size.dir/llvm-size.cpp.i"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-size && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/llvm-size/llvm-size.cpp > CMakeFiles/llvm-size.dir/llvm-size.cpp.i

tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/llvm-size.dir/llvm-size.cpp.s"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-size && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/llvm-size/llvm-size.cpp -o CMakeFiles/llvm-size.dir/llvm-size.cpp.s

tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o.requires:
.PHONY : tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o.requires

tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o.provides: tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o.requires
	$(MAKE) -f tools/llvm-size/CMakeFiles/llvm-size.dir/build.make tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o.provides.build
.PHONY : tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o.provides

tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o.provides.build: tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o

# Object files for target llvm-size
llvm__size_OBJECTS = \
"CMakeFiles/llvm-size.dir/llvm-size.cpp.o"

# External object files for target llvm-size
llvm__size_EXTERNAL_OBJECTS =

bin/llvm-size: tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o
bin/llvm-size: tools/llvm-size/CMakeFiles/llvm-size.dir/build.make
bin/llvm-size: lib/libLLVMObject.a
bin/llvm-size: lib/libLLVMSupport.a
bin/llvm-size: lib/libLLVMBitReader.a
bin/llvm-size: lib/libLLVMCore.a
bin/llvm-size: lib/libLLVMMCParser.a
bin/llvm-size: lib/libLLVMMC.a
bin/llvm-size: lib/libLLVMSupport.a
bin/llvm-size: tools/llvm-size/CMakeFiles/llvm-size.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/llvm-size"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-size && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/llvm-size.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/llvm-size/CMakeFiles/llvm-size.dir/build: bin/llvm-size
.PHONY : tools/llvm-size/CMakeFiles/llvm-size.dir/build

tools/llvm-size/CMakeFiles/llvm-size.dir/requires: tools/llvm-size/CMakeFiles/llvm-size.dir/llvm-size.cpp.o.requires
.PHONY : tools/llvm-size/CMakeFiles/llvm-size.dir/requires

tools/llvm-size/CMakeFiles/llvm-size.dir/clean:
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-size && $(CMAKE_COMMAND) -P CMakeFiles/llvm-size.dir/cmake_clean.cmake
.PHONY : tools/llvm-size/CMakeFiles/llvm-size.dir/clean

tools/llvm-size/CMakeFiles/llvm-size.dir/depend:
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/llvm-size /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-size /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-size/CMakeFiles/llvm-size.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/llvm-size/CMakeFiles/llvm-size.dir/depend

