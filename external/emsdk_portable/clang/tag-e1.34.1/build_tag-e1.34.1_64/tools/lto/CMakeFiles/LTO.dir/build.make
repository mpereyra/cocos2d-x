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
include tools/lto/CMakeFiles/LTO.dir/depend.make

# Include the progress variables for this target.
include tools/lto/CMakeFiles/LTO.dir/progress.make

# Include the compile flags for this target's objects.
include tools/lto/CMakeFiles/LTO.dir/flags.make

tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o: tools/lto/CMakeFiles/LTO.dir/flags.make
tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/lto/LTODisassembler.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/lto && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/LTO.dir/LTODisassembler.cpp.o -c /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/lto/LTODisassembler.cpp

tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LTO.dir/LTODisassembler.cpp.i"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/lto && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/lto/LTODisassembler.cpp > CMakeFiles/LTO.dir/LTODisassembler.cpp.i

tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LTO.dir/LTODisassembler.cpp.s"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/lto && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/lto/LTODisassembler.cpp -o CMakeFiles/LTO.dir/LTODisassembler.cpp.s

tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o.requires:
.PHONY : tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o.requires

tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o.provides: tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o.requires
	$(MAKE) -f tools/lto/CMakeFiles/LTO.dir/build.make tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o.provides.build
.PHONY : tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o.provides

tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o.provides.build: tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o

tools/lto/CMakeFiles/LTO.dir/lto.cpp.o: tools/lto/CMakeFiles/LTO.dir/flags.make
tools/lto/CMakeFiles/LTO.dir/lto.cpp.o: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/lto/lto.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tools/lto/CMakeFiles/LTO.dir/lto.cpp.o"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/lto && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/LTO.dir/lto.cpp.o -c /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/lto/lto.cpp

tools/lto/CMakeFiles/LTO.dir/lto.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LTO.dir/lto.cpp.i"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/lto && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/lto/lto.cpp > CMakeFiles/LTO.dir/lto.cpp.i

tools/lto/CMakeFiles/LTO.dir/lto.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LTO.dir/lto.cpp.s"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/lto && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/lto/lto.cpp -o CMakeFiles/LTO.dir/lto.cpp.s

tools/lto/CMakeFiles/LTO.dir/lto.cpp.o.requires:
.PHONY : tools/lto/CMakeFiles/LTO.dir/lto.cpp.o.requires

tools/lto/CMakeFiles/LTO.dir/lto.cpp.o.provides: tools/lto/CMakeFiles/LTO.dir/lto.cpp.o.requires
	$(MAKE) -f tools/lto/CMakeFiles/LTO.dir/build.make tools/lto/CMakeFiles/LTO.dir/lto.cpp.o.provides.build
.PHONY : tools/lto/CMakeFiles/LTO.dir/lto.cpp.o.provides

tools/lto/CMakeFiles/LTO.dir/lto.cpp.o.provides.build: tools/lto/CMakeFiles/LTO.dir/lto.cpp.o

# Object files for target LTO
LTO_OBJECTS = \
"CMakeFiles/LTO.dir/LTODisassembler.cpp.o" \
"CMakeFiles/LTO.dir/lto.cpp.o"

# External object files for target LTO
LTO_EXTERNAL_OBJECTS =

lib/libLTO.3.7.0svn.dylib: tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o
lib/libLTO.3.7.0svn.dylib: tools/lto/CMakeFiles/LTO.dir/lto.cpp.o
lib/libLTO.3.7.0svn.dylib: tools/lto/CMakeFiles/LTO.dir/build.make
lib/libLTO.3.7.0svn.dylib: lib/libLLVMX86CodeGen.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMX86AsmPrinter.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMX86AsmParser.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMX86Desc.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMX86Info.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMX86Disassembler.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMJSBackendCodeGen.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMJSBackendDesc.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMJSBackendInfo.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMCore.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMLTO.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMMC.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMMCDisassembler.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMNaClTransforms.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMSupport.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMX86CodeGen.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMAsmPrinter.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMSelectionDAG.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMX86Desc.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMX86AsmPrinter.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMX86Utils.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMX86Info.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMMCDisassembler.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMCodeGen.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMTarget.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMBitWriter.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMLinker.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMObjCARCOpts.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMipo.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMScalarOpts.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMInstCombine.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMProfileData.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMObject.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMMCParser.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMMC.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMBitReader.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMVectorize.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMTransformUtils.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMipa.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMAnalysis.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMCore.a
lib/libLTO.3.7.0svn.dylib: lib/libLLVMSupport.a
lib/libLTO.3.7.0svn.dylib: tools/lto/CMakeFiles/LTO.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library ../../lib/libLTO.dylib"
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/lto && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LTO.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/lto && $(CMAKE_COMMAND) -E cmake_symlink_library ../../lib/libLTO.3.7.0svn.dylib ../../lib/libLTO.3.7.dylib ../../lib/libLTO.dylib

lib/libLTO.3.7.dylib: lib/libLTO.3.7.0svn.dylib

lib/libLTO.dylib: lib/libLTO.3.7.0svn.dylib

# Rule to build all files generated by this target.
tools/lto/CMakeFiles/LTO.dir/build: lib/libLTO.dylib
.PHONY : tools/lto/CMakeFiles/LTO.dir/build

tools/lto/CMakeFiles/LTO.dir/requires: tools/lto/CMakeFiles/LTO.dir/LTODisassembler.cpp.o.requires
tools/lto/CMakeFiles/LTO.dir/requires: tools/lto/CMakeFiles/LTO.dir/lto.cpp.o.requires
.PHONY : tools/lto/CMakeFiles/LTO.dir/requires

tools/lto/CMakeFiles/LTO.dir/clean:
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/lto && $(CMAKE_COMMAND) -P CMakeFiles/LTO.dir/cmake_clean.cmake
.PHONY : tools/lto/CMakeFiles/LTO.dir/clean

tools/lto/CMakeFiles/LTO.dir/depend:
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/lto /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/lto /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/lto/CMakeFiles/LTO.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/lto/CMakeFiles/LTO.dir/depend
