# Install script for directory: /Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/cmake/modules

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/llvm/cmake/LLVMExports.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/llvm/cmake/LLVMExports.cmake"
         "/Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/cmake/modules/CMakeFiles/Export/share/llvm/cmake/LLVMExports.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/llvm/cmake/LLVMExports-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/llvm/cmake/LLVMExports.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/llvm/cmake" TYPE FILE FILES "/Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/cmake/modules/CMakeFiles/Export/share/llvm/cmake/LLVMExports.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/llvm/cmake" TYPE FILE FILES "/Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/cmake/modules/CMakeFiles/Export/share/llvm/cmake/LLVMExports-relwithdebinfo.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/llvm/cmake" TYPE FILE FILES
    "/Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/cmake/modules/CMakeFiles/LLVMConfig.cmake"
    "/Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/share/llvm/cmake/LLVMConfigVersion.cmake"
    "/Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/cmake/modules/LLVM-Config.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/llvm/cmake" TYPE DIRECTORY FILES "/Users/danieldionne/git/fg/lib/griffin/lib/cocos2d-x/external/emsdk_portable/clang/tag-e1.34.1/src/cmake/modules/." FILES_MATCHING REGEX "/[^/]*\\.cmake$" REGEX "/\\.svn$" EXCLUDE REGEX "/llvmconfig\\.cmake$" EXCLUDE REGEX "/llvmconfigversion\\.cmake$" EXCLUDE REGEX "/llvm\\-config\\.cmake$" EXCLUDE REGEX "/gethosttriple\\.cmake$" EXCLUDE REGEX "/versionfromvcs\\.cmake$" EXCLUDE REGEX "/checkatomic\\.cmake$" EXCLUDE)
endif()

