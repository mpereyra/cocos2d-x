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

# Utility rule file for ClangCommentHTMLTags.

# Include the progress variables for this target.
include tools/clang/include/clang/AST/CMakeFiles/ClangCommentHTMLTags.dir/progress.make

tools/clang/include/clang/AST/CMakeFiles/ClangCommentHTMLTags: tools/clang/include/clang/AST/CommentHTMLTags.inc

tools/clang/include/clang/AST/CommentHTMLTags.inc: tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Updating CommentHTMLTags.inc..."
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST && /usr/local/Cellar/cmake/3.0.2/bin/cmake -E copy_if_different /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST/CommentHTMLTags.inc

tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: bin/clang-tblgen
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST/CommentCommands.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST/CommentHTMLNamedCharacterReferences.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST/CommentHTMLTags.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/CodeGen/ValueTypes.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/Intrinsics.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsAArch64.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsARM.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsBPF.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsHexagon.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsMips.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsNVVM.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsPowerPC.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsR600.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsSystemZ.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsX86.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsXCore.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Option/OptParser.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Target/Target.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Target/TargetCallingConv.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Target/TargetItinerary.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Target/TargetSchedule.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Target/TargetSelectionDAG.td
tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST/CommentHTMLTags.td
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Building CommentHTMLTags.inc..."
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST && ../../../../../bin/clang-tblgen -gen-clang-comment-html-tags -I /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST -I /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/lib/Target -I /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/include /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST/CommentHTMLTags.td -o /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp

ClangCommentHTMLTags: tools/clang/include/clang/AST/CMakeFiles/ClangCommentHTMLTags
ClangCommentHTMLTags: tools/clang/include/clang/AST/CommentHTMLTags.inc
ClangCommentHTMLTags: tools/clang/include/clang/AST/CommentHTMLTags.inc.tmp
ClangCommentHTMLTags: tools/clang/include/clang/AST/CMakeFiles/ClangCommentHTMLTags.dir/build.make
.PHONY : ClangCommentHTMLTags

# Rule to build all files generated by this target.
tools/clang/include/clang/AST/CMakeFiles/ClangCommentHTMLTags.dir/build: ClangCommentHTMLTags
.PHONY : tools/clang/include/clang/AST/CMakeFiles/ClangCommentHTMLTags.dir/build

tools/clang/include/clang/AST/CMakeFiles/ClangCommentHTMLTags.dir/clean:
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST && $(CMAKE_COMMAND) -P CMakeFiles/ClangCommentHTMLTags.dir/cmake_clean.cmake
.PHONY : tools/clang/include/clang/AST/CMakeFiles/ClangCommentHTMLTags.dir/clean

tools/clang/include/clang/AST/CMakeFiles/ClangCommentHTMLTags.dir/depend:
	cd /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST/CMakeFiles/ClangCommentHTMLTags.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/clang/include/clang/AST/CMakeFiles/ClangCommentHTMLTags.dir/depend
