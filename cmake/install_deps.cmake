#!/usr/bin/env -S cmake -P

# This script installs external dependencies.

cmake_minimum_required(VERSION 3.22)
set(CMAKE_MESSAGE_CONTEXT_SHOW ON)

# Get a unique prefix for variables.
set(current_list_file_path "${CMAKE_CURRENT_LIST_FILE}")
cmake_path(GET current_list_file_path STEM script_name)

if(NOT ${script_name}_INSTALL_PREFIX)
  set(${script_name}_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/_deps")
endif()
message("install prefix: ${${script_name}_INSTALL_PREFIX}")

file(GLOB result "${${script_name}_INSTALL_PREFIX}/*")
if(result)
  message(
    FATAL_ERROR
      "Directory ${${script_name}_INSTALL_PREFIX} is not empty: ${result}")
endif()

# Setup git.
if(NOT ${script_name}_GIT_PROGRAM)
  find_program(${script_name}_GIT_PROGRAM git REQUIRED)

  execute_process(
    COMMAND ${${script_name}_GIT_PROGRAM} --version
    RESULT_VARIABLE result
    OUTPUT_VARIABLE stdout
    ERROR_VARIABLE stderr)

  if(result)
    message(FATAL_ERROR "'${${script_name}_GIT_PROGRAM} --version' "
                        "failed with stdout: ${stdout}" "and stderr: ${stderr}")
  endif()

  string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" gitVersion ${stdout})
  message(STATUS "Using git ${gitVersion}")
endif()

include(ProcessorCount)
ProcessorCount(n_cores)
if(n_cores EQUAL 0)
  set(recurse_cores "")
else()
  set(recurse_cores --recurse-submodules=${n_cores})
endif()

function(install_deps_git_clone)
  list(APPEND CMAKE_MESSAGE_CONTEXT "install_deps_git_clone")

  set(flags)
  set(args TAG URL)
  set(list_args)
  cmake_parse_arguments(arg "${flags}" "${args}" "${list_args}" ${ARGN})

  foreach(argument IN ITEMS TAG URL)
    if(NOT arg_${argument})
      message(FATAL_ERROR "${argument} is a required argument")
    endif()
    if(${argument} IN_LIST arg_KEYWORDS_MISSING_VALUES)
      message(FATAL_ERROR "${argument} requires at least one value")
    endif()
  endforeach()

  # cmake-format: off
  set(command ${${script_name}_GIT_PROGRAM} -c advice.detachedHead=false
              clone --progress ${recurse_cores} -b ${arg_TAG} ${arg_URL})
  # cmake-format: on

  execute_process(COMMAND ${command} COMMAND_ECHO STDOUT ECHO_OUTPUT_VARIABLE
                          ECHO_ERROR_VARIABLE COMMAND_ERROR_IS_FATAL ANY)

  list(POP_BACK CMAKE_MESSAGE_CONTEXT)
endfunction()

function(install_deps_cmake_configure)
  list(APPEND CMAKE_MESSAGE_CONTEXT "install_deps_cmake_configure")

  set(flags)
  set(args PROJECT BUILD_DIR INSTALL_DIR)
  set(list_args CMAKE_DEFINITIONS)
  cmake_parse_arguments(arg "${flags}" "${args}" "${list_args}" ${ARGN})

  foreach(argument IN ITEMS PROJECT BUILD_DIR INSTALL_DIR)
    if(NOT arg_${argument})
      message(FATAL_ERROR "${argument} is a required argument")
    endif()
    if(${argument} IN_LIST arg_KEYWORDS_MISSING_VALUES)
      message(FATAL_ERROR "${argument} requires at least one value")
    endif()
  endforeach()

  foreach(def IN LISTS arg_CMAKE_DEFINITIONS)
    string(REPLACE ";" " " defs ${def})
  endforeach()

  # cmake-format: off
  set(command
      "${CMAKE_COMMAND}" -S . -B "${arg_BUILD_DIR}"
      -DCMAKE_BUILD_TYPE=Release
      "-DCMAKE_INSTALL_PREFIX=${arg_INSTALL_DIR}"
      ${defs})
  # cmake-format: on

  execute_process(
    COMMAND ${command}
    WORKING_DIRECTORY
      ${arg_PROJECT}
      COMMAND_ECHO
      STDOUT
      ECHO_OUTPUT_VARIABLE
      ECHO_ERROR_VARIABLE
      COMMAND_ERROR_IS_FATAL
      ANY)

  list(POP_BACK CMAKE_MESSAGE_CONTEXT)
endfunction()

function(install_deps_cmake_build_install)
  list(APPEND CMAKE_MESSAGE_CONTEXT "install_deps_cmake_build_install")

  set(flags)
  set(args PROJECT BUILD_DIR)
  set(list_args)
  cmake_parse_arguments(arg "${flags}" "${args}" "${list_args}" ${ARGN})

  foreach(argument IN ITEMS PROJECT BUILD_DIR)
    if(NOT arg_${argument})
      message(FATAL_ERROR "${argument} is a required argument")
    endif()
    if(${argument} IN_LIST arg_KEYWORDS_MISSING_VALUES)
      message(FATAL_ERROR "${argument} requires at least one value")
    endif()
  endforeach()

  # cmake-format: off
  set(command ${CMAKE_COMMAND} --build ${arg_BUILD_DIR}
              -- -j${n_cores} install)
  # cmake-format: on

  execute_process(
    COMMAND ${command}
    WORKING_DIRECTORY
      ${arg_PROJECT}
      COMMAND_ECHO
      STDOUT
      ECHO_OUTPUT_VARIABLE
      ECHO_ERROR_VARIABLE
      COMMAND_ERROR_IS_FATAL
      ANY)

  list(POP_BACK CMAKE_MESSAGE_CONTEXT)
endfunction()

# cmake-format: off
message(STATUS "Installing spdlog")
install_deps_git_clone(URL https://github.com/gabime/spdlog.git TAG v1.10.0)
set(cmake_definitions -DSPDLOG_BUILD_EXAMPLE=OFF -DSPDLOG_FMT_EXTERNAL=ON)
install_deps_cmake_configure(PROJECT spdlog
                             BUILD_DIR "build_v1.10.0"
                             INSTALL_DIR "${${script_name}_INSTALL_PREFIX}"
                             CMAKE_DEFINITIONS "${cmake_definitions}")
install_deps_cmake_build_install(PROJECT spdlog BUILD_DIR "build_v1.10.0")

message(STATUS "Installing fmt")
install_deps_git_clone(URL https://github.com/fmtlib/fmt.git TAG 8.1.1)
set(cmake_definitions -DFMT_TEST=OFF)
install_deps_cmake_configure(PROJECT fmt
                             BUILD_DIR "build_8.1.1"
                             INSTALL_DIR "${${script_name}_INSTALL_PREFIX}"
                             CMAKE_DEFINITIONS "${cmake_definitions}")
install_deps_cmake_build_install(PROJECT fmt BUILD_DIR "build_8.1.1")

message(STATUS "Installing googletest")
install_deps_git_clone(URL https://github.com/google/googletest.git TAG release-1.12.1)
install_deps_cmake_configure(PROJECT googletest
                             BUILD_DIR "build_release-1.12.1"
                             INSTALL_DIR "${${script_name}_INSTALL_PREFIX}")
install_deps_cmake_build_install(PROJECT googletest BUILD_DIR "build_release-1.12.1")
# cmake-format: on

message(STATUS "Cleaning up")

# cmake-format: off
execute_process(
  COMMAND "${CMAKE_COMMAND}" -E rm -rf spdlog fmt googletest
  COMMAND_ECHO STDOUT
  ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE
  COMMAND_ERROR_IS_FATAL ANY)
# cmake-format: on

message(STATUS "Done")
