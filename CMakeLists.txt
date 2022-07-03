cmake_minimum_required(VERSION 3.22)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

project(wors_tracer LANGUAGES CXX)

set(${PROJECT_NAME}_LOGGING_LEVEL
    "Trace"
    CACHE STRING "Compile-time logging level.")
# cmake-format: off
set(${PROJECT_NAME}_logging_levels
    "Trace" "Debug" "Info" "Warning" "Error" "Critical" "Off")
# cmake-format: on
set_property(CACHE ${PROJECT_NAME}_LOGGING_LEVEL
             PROPERTY STRINGS "${${PROJECT_NAME}_logging_levels}")

if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

  set(CMAKE_CXX_EXTENSIONS OFF)

  set(buildTypes Debug Release RelWithDebInfo Testing Fast Coverage)

  include(util)
  util_set_allowed_build_types(BUILD_TYPES ${buildTypes})
  util_reset_cxx_flags_for(BUILD_TYPES ${buildTypes})

  add_library(${PROJECT_NAME}_common_flags INTERFACE)

  set(CMAKE_REQUIRED_LINK_OPTIONS -fsanitize=address,undefined)
  util_check_cxx_flag(FLAG -fsanitize=address,undefined)
  unset(CMAKE_REQUIRED_LINK_OPTIONS)
  set(hasAsanUbsan
      $<BOOL:${PROJECT_NAME}_SUPPORTS_FLAG_fsanitize_address_undefined>)
  if(NOT ${PROJECT_NAME}_SUPPORTS_FLAG_fsanitize_address_undefined)
    message(WARNING "Tests will run without Google ASAN and UBSAN.")
  endif()

  util_check_cxx_flag(FLAG -Og)
  set(hasOg $<BOOL:${PROJECT_NAME}_SUPPORTS_FLAG_Og>)

  set(isDebug $<CONFIG:Debug>)
  set(isRelease $<CONFIG:Release>)
  set(isRelWithDebInfo $<CONFIG:RelWithDebInfo>)
  set(isTesting $<CONFIG:Testing>)
  set(isFast $<CONFIG:Fast>)
  set(isCoverage $<CONFIG:Coverage>)
  set(isGNU $<CXX_COMPILER_ID:GNU>)
  set(isClang $<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>)
  set(isGNUorClang $<OR:${isGNU},${isClang}>)

  target_compile_options(
    ${PROJECT_NAME}_common_flags
    INTERFACE
      $<${isGNU}:-fdiagnostics-color=always>
      $<${isClang}:-fcolor-diagnostics>
      $<${isGNUorClang}:-Wall>
      $<${isGNUorClang}:-Wextra>
      $<${isGNUorClang}:-Wstrict-aliasing>
      $<${isDebug}:$<${isGNUorClang}:$<IF:${hasOg},-Og,-g>>>
      $<${isDebug}:$<${isGNUorClang}:-ggdb3>>
      $<${isRelease}:$<${isGNUorClang}:-O3>>
      $<$<OR:${isRelWithDebInfo},${isTesting}>:$<${isGNUorClang}:-O2>>
      $<$<OR:${isRelWithDebInfo},${isTesting}>:$<${isGNUorClang}:-ggdb3>>
      $<${isTesting}:$<${isGNUorClang}:$<${hasAsanUbsan}:-fsanitize=address,undefined>>>
      $<${isTesting}:$<${isGNUorClang}:$<${hasAsanUbsan}:-fno-omit-frame-pointer>>>
      $<${isFast}:$<${isGNUorClang}:-Ofast>>
      $<${isCoverage}:$<${isGNUorClang}:-ggdb3>>
      $<${isCoverage}:$<${isGNUorClang}:-O0>>
      $<${isCoverage}:$<${isGNUorClang}:-fno-omit-frame-pointer>>)

  target_compile_definitions(
    ${PROJECT_NAME}_common_flags
    INTERFACE $<$<OR:${isRelease},${isFast}>:$<${isGNUorClang}:NDEBUG>>)

  target_link_options(
    ${PROJECT_NAME}_common_flags
    INTERFACE
    $<$<AND:${isTesting},${isGNUorClang},${hasAsanUbsan}>:-fsanitize=address,undefined>
  )
endif() # PROJECT_IS_TOPLEVEL

include(FindPackageMessage)
find_package(fmt CONFIG REQUIRED)
find_package_message(fmt "Using fmt ${fmt_VERSION}" "[${fmt_VERSION}]")
find_package(spdlog CONFIG REQUIRED)
find_package_message(spdlog "Using spdlog ${spdlog_VERSION}"
                     "[${spdlog_VERSION}]")

add_executable(${PROJECT_NAME} main.cc log.cc)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE $<TARGET_NAME_IF_EXISTS:${PROJECT_NAME}_common_flags>
                          spdlog::spdlog fmt::fmt m)

if(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Trace")
  target_compile_definitions(${PROJECT_NAME}
                             PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Debug")
  target_compile_definitions(${PROJECT_NAME}
                             PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Info")
  target_compile_definitions(${PROJECT_NAME}
                             PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Warning")
  target_compile_definitions(${PROJECT_NAME}
                             PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_WARN)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Error")
  target_compile_definitions(${PROJECT_NAME}
                             PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_ERROR)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Critical")
  target_compile_definitions(${PROJECT_NAME}
                             PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_CRITICAL)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Off")
  target_compile_definitions(${PROJECT_NAME}
                             PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_OFF)
else()
  message(
    FATAL_ERROR
      "${PROJECT_NAME}_LOGGING_LEVEL has incorrect value. "
      "Available logging levels are: ${${PROJECT_NAME}_logging_levels}")
endif()

if(${PROJECT_NAME}_BUILD_TESTING OR PROJECT_IS_TOP_LEVEL)
  enable_testing()

  find_package(GTest CONFIG REQUIRED)
  find_package_message(GTest "Using GTest ${GTest_VERSION}"
                       "[${GTest_VERSION}]")

  add_library(${PROJECT_NAME}_test_base INTERFACE)
  target_sources(${PROJECT_NAME}_test_base INTERFACE vec3_test.cc vec4_test.cc
                                                     mat3_test.cc mat4_test.cc)
  target_include_directories(${PROJECT_NAME}_test_base INTERFACE .)
  target_compile_features(${PROJECT_NAME}_test_base INTERFACE cxx_std_20)
  target_compile_options(
    ${PROJECT_NAME}_test_base
    INTERFACE $<${isCoverage}:$<${isGNUorClang}:--coverage>>)
  target_link_libraries(
    ${PROJECT_NAME}_test_base
    INTERFACE $<TARGET_NAME_IF_EXISTS:${PROJECT_NAME}_common_flags>
              $<${isCoverage}:$<${isGNUorClang}:--coverage>> GTest::gtest
              GTest::gtest_main)

  add_executable(${PROJECT_NAME}_test)
  target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_test_base)
endif() # ${PROJECT_NAME}_BUILD_TESTING OR PROJECT_IS_TOP_LEVEL
