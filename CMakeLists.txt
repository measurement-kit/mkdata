cmake_minimum_required(VERSION 3.1.0)
project(mkdata LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
include(MkUtils)

# Download dependencies
# ---------------------

MkDownloadCatchorgCatch2()

# Compiler flags
# --------------

MkSetCompilerFlags()

# Library and binary
# ------------------

set(MKDATA_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_REQUIRED_INCLUDES})
include_directories(${MKDATA_INCLUDES})

if("${WIN32}" OR "${MINGW}")
  list(APPEND MKDATA_LIBS "ws2_32")
  if ("${MINGW}")
      list(APPEND MKDATA_LIBS -static-libgcc -static-libstdc++)
  endif()
endif()
list(APPEND MKDATA_LIBS Threads::Threads)
link_libraries("${MKDATA_LIBS}")

install(FILES mkdata.h DESTINATION include/measurement-kit)

# Testing
# -------

set(BUILD_TESTING "ON" CACHE BOOL "Whether to build tests")
if(${BUILD_TESTING})
  add_executable(unit-tests unit-tests.cpp)
  enable_testing()
  add_test(NAME unit_tests COMMAND unit-tests)
endif()
