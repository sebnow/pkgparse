cmake_minimum_required(VERSION 2.8)

project(pkgparse C)
set(pkgparse_VERSION_MAJOR 0)
set(pkgparse_VERSION_MINOR 1)
set(pkgparse_VERSION ${pkgparse_VERSION_MAJOR}.${pkgparse_VERSION_MINOR})

find_package(BISON)
find_package(FLEX)

BISON_TARGET(pkgbuild_parser pkgbuild_parse.y ${CMAKE_CURRENT_BINARY_DIR}/pkgbuild_parse.c)
FLEX_TARGET(pkgbuild_scanner pkgbuild_scanner.l ${CMAKE_CURRENT_BINARY_DIR}/pkgbuild_scanner.c)
ADD_FLEX_BISON_DEPENDENCY(pkgbuild_scanner pkgbuild_parser)

set(pkgparse_SRCS
  pkgbuild.c
  symbol.c
  utility.c
  ${BISON_pkgbuild_parser_OUTPUTS}
  ${FLEX_pkgbuild_scanner_OUTPUTS}
)

set(test_SRCS
  test_runner.c
  pkgbuild_test.c
  symbol_test.c
  utility_test.c
)

include(CheckCCompilerFlag)
CHECK_C_COMPILER_FLAG(-fvisibility=hidden GCC_VISIBILITY_HIDDEN)
CHECK_C_COMPILER_FLAG(-fvisibility=internal GCC_VISIBILITY_INTERNAL)

if(GCC_VISIBILITY_INTERNAL AND NOT APPLE)
  set(pkgparse_GCC_VISIBILITY_FLAGS "-fvisibility=internal")
elseif(GCC_VISIBILITY_HIDDEN)
  set(pkgparse_GCC_VISIBILITY_FLAGS "-fvisibility=hidden")
else(GCC_VISIBILITY_HIDDEN)
  set(pkgparse_GCC_VISIBILITY_FLAGS "")
endif(GCC_VISIBILITY_INTERNAL AND NOT APPLE)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(pkgparse_CFLAGS ${pkgparse_GCC_VISIBILITY_FLAGS})

add_library(pkgparse ${pkgparse_SRCS})
set_target_properties(pkgparse PROPERTIES VERSION ${pkgparse_VERSION} SOVERSION ${pkgparse_VERSION_MAJOR})
set_target_properties(pkgparse PROPERTIES COMPILE_FLAGS ${pkgparse_CFLAGS})

include(CheckLibraryExists)
check_library_exists(cmockery _assert_true "" HAVE_CMOCKERY)

# Unit tests
if(HAVE_CMOCKERY)
  add_custom_target(test COMMAND ./test_runner)
  if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
  endif(COMMAND cmake_policy)
  add_executable(test_runner EXCLUDE_FROM_ALL ${test_SRCS} ${pkgparse_SRCS})
  add_test(test_runner test_runner)
  add_dependencies(test test_runner)
  find_library(cmockery_LIBRARY NAMES cmockery)
  set(cmockery_PROCESS_LIBS cmockery_LIBRARY cmockery_LIBRARIES)
  target_link_libraries(test_runner pkgparse cmockery)
endif(HAVE_CMOCKERY)
