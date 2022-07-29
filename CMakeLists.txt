cmake_minimum_required(VERSION 3.22)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/_deps")
if(UNIX AND NOT APPLE)
  list(APPEND CMAKE_PREFIX_PATH "/usr/lib/cmake")
endif()

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

set(${PROJECT_NAME}_LTO
    OFF
    CACHE BOOL "Use link-time optimization. \
      Set _CMAKE_TOOLCHAIN_PREFIX to gcc- or llvm- manually.")

# To use this, first run CMake with =Generate, run the program, then with =Use
set(${PROJECT_NAME}_PGO
    "Off"
    CACHE STRING "Use profile guided optimization. Use -j 1.")
set_property(CACHE ${PROJECT_NAME}_PGO PROPERTY STRINGS "Off;Generate;Use")

set(${PROJECT_NAME}_NATIVE
    OFF
    CACHE BOOL "Use -march=native")

if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

  set(CMAKE_CXX_EXTENSIONS OFF)

  set(buildTypes
      Debug
      Release
      RelWithDebInfo
      Testing
      Fast
      Coverage
      Profile)

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
  set(hasOg $<BOOL:${${PROJECT_NAME}_SUPPORTS_FLAG_Og}>)

  set(isDebug $<CONFIG:Debug>)
  set(isRelease $<CONFIG:Release>)
  set(isRelWithDebInfo $<CONFIG:RelWithDebInfo>)
  set(isTesting $<CONFIG:Testing>)
  set(isFast $<CONFIG:Fast>)
  set(isCoverage $<CONFIG:Coverage>)
  set(isProfile $<CONFIG:Profile>)
  set(isGNU $<CXX_COMPILER_ID:GNU>)
  set(isClang $<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>)
  set(isGNUorClang $<OR:${isGNU},${isClang}>)

  set(useLto $<BOOL:${${PROJECT_NAME}_LTO}>)
  set(useNative $<BOOL:${${PROJECT_NAME}_NATIVE}>)
  set(PgoGenerate $<STREQUAL:${${PROJECT_NAME}_PGO},Generate>)
  set(PgoUse $<STREQUAL:${${PROJECT_NAME}_PGO},Use>)

  target_compile_options(
    ${PROJECT_NAME}_common_flags
    INTERFACE
      $<${isGNU}:-fdiagnostics-color=always>
      $<${isClang}:-fcolor-diagnostics>
      $<${isGNUorClang}:-Wall>
      $<${isGNUorClang}:-Wextra>
      $<${isGNUorClang}:-Wstrict-aliasing>
      $<${isDebug}:$<${isGNUorClang}:$<IF:${hasOg},-Og,>>>
      $<${isDebug}:$<${isGNUorClang}:-ggdb3>>
      $<${isRelease}:$<${isGNUorClang}:-O3>>
      $<$<OR:${isRelWithDebInfo},${isTesting}>:$<${isGNUorClang}:-O2>>
      $<$<OR:${isRelWithDebInfo},${isTesting}>:$<${isGNUorClang}:-ggdb3>>
      $<${isTesting}:$<${isGNUorClang}:$<${hasAsanUbsan}:-fsanitize=address,undefined>>>
      $<${isTesting}:$<${isGNUorClang}:$<${hasAsanUbsan}:-fno-omit-frame-pointer>>>
      $<${isProfile}:$<${isGNUorClang}:-O0>>
      $<${isProfile}:$<${isGNUorClang}:-ggdb3>>
      $<${isProfile}:$<${isGNUorClang}:-fno-omit-frame-pointer>>
      $<${isFast}:$<${isGNUorClang}:-Ofast>>
      $<${isCoverage}:$<${isGNUorClang}:-ggdb3>>
      $<${isCoverage}:$<${isGNUorClang}:-fno-omit-frame-pointer>>
      $<${useLto}:$<${isGNUorClang}:-flto=auto>>
      $<${useNative}:$<${isGNUorClang}:-march=native>>
      $<${PgoGenerate}:-fprofile-generate>
      $<${PgoUse}:-fprofile-use>
      $<${PgoUse}:-fprofile-correction>)

  target_compile_definitions(
    ${PROJECT_NAME}_common_flags
    INTERFACE $<$<OR:${isRelease},${isFast}>:$<${isGNUorClang}:NDEBUG>>)

  target_link_options(
    ${PROJECT_NAME}_common_flags
    INTERFACE
    $<${isClang}:-fuse-ld=lld>
    $<$<AND:${isTesting},${isGNUorClang},${hasAsanUbsan}>:-fsanitize=address,undefined>
    $<${isDebug}:$<${isGNUorClang}:$<IF:${hasOg},-Og,>>>
    $<${isRelease}:$<${isGNUorClang}:-O3>>
    $<$<OR:${isRelWithDebInfo},${isTesting}>:$<${isGNUorClang}:-O2>>
    $<${isFast}:$<${isGNUorClang}:-Ofast>>
    $<${useLto}:$<${isGNUorClang}:-flto=auto>>
    $<${useNative}:$<${isGNUorClang}:-march=native>>
    $<${PgoGenerate}:-fprofile-generate>
    $<${PgoUse}:-fprofile-use>
    $<${PgoUse}:-fprofile-correction>)
endif() # PROJECT_IS_TOPLEVEL

include(FindPackageMessage)
find_package(fmt CONFIG REQUIRED)
find_package_message(fmt "Using fmt ${fmt_VERSION}" "[${fmt_VERSION}]")
find_package(spdlog CONFIG REQUIRED)
find_package_message(spdlog "Using spdlog ${spdlog_VERSION}"
                     "[${spdlog_VERSION}]")
find_package(TBB CONFIG REQUIRED)
find_package_message(TBB "Using TBB ${spdlog_VERSION}"
                     "[${spdlog_VERSION}]")

add_library(
  lib${PROJECT_NAME}
  mat3.cc
  mat4.cc
  vec3.cc
  vec4.cc
  ray.cc
  pnt3.cc
  pnt4.cc
  tform4.cc
  canvas.cc
  color.cc
  util.cc
  sphere.cc
  intersection.cc
  lighting.cc
  world.cc
  shading.cc
  camera.cc
  render.cc
  shape.cc
  shape_concept.cc
  shape_model.cc
  sphere.cc
  sphere_operations.cc)

target_include_directories(lib${PROJECT_NAME} PUBLIC .)
target_compile_definitions(lib${PROJECT_NAME} PUBLIC SPDLOG_FMT_EXTERNAL)
target_compile_features(lib${PROJECT_NAME} PUBLIC cxx_std_20)
if(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Trace")
  target_compile_definitions(lib${PROJECT_NAME}
                             PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Debug")
  target_compile_definitions(lib${PROJECT_NAME}
                             PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Info")
  target_compile_definitions(lib${PROJECT_NAME}
                             PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Warning")
  target_compile_definitions(lib${PROJECT_NAME}
                             PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_WARN)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Error")
  target_compile_definitions(lib${PROJECT_NAME}
                             PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_ERROR)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Critical")
  target_compile_definitions(lib${PROJECT_NAME}
                             PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_CRITICAL)
elseif(${PROJECT_NAME}_LOGGING_LEVEL STREQUAL "Off")
  target_compile_definitions(lib${PROJECT_NAME}
                             PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_OFF)
else()
  message(
    FATAL_ERROR
      "${PROJECT_NAME}_LOGGING_LEVEL has incorrect value. "
      "Available logging levels are: ${${PROJECT_NAME}_logging_levels}")
endif()
target_link_libraries(
  lib${PROJECT_NAME}
  PUBLIC $<TARGET_NAME_IF_EXISTS:${PROJECT_NAME}_common_flags> m fmt::fmt
         spdlog::spdlog TBB::tbb)

add_executable(${PROJECT_NAME} main.cc log.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE lib${PROJECT_NAME})

if(${PROJECT_NAME}_BUILD_TESTING OR PROJECT_IS_TOP_LEVEL)
  enable_testing()

  find_package(GTest CONFIG REQUIRED)
  find_package_message(GTest "Using GTest ${GTest_VERSION}"
                       "[${GTest_VERSION}]")

  add_library(${PROJECT_NAME}_test_base INTERFACE)

  target_sources(
    ${PROJECT_NAME}_test_base
    INTERFACE vec3_test.cc
              ray_test.cc
              vec4_test.cc
              mat3_test.cc
              mat4_test.cc
              # sphere_test.cc
              pnt3_test.cc
              canvas_test.cc
              color_test.cc
              normal_test.cc
              tform_test.cc
              world_test.cc
              shading_test.cc
              camera_test.cc
              render_test.cc
              lighting_test.cc
              test_main.cc
              log.cc)

  target_compile_options(
    ${PROJECT_NAME}_test_base
    INTERFACE $<${isCoverage}:$<${isGNUorClang}:--coverage>>)
  target_link_libraries(
    ${PROJECT_NAME}_test_base
    INTERFACE $<TARGET_NAME_IF_EXISTS:${PROJECT_NAME}_common_flags>
              $<${isCoverage}:$<${isGNUorClang}:--coverage>> lib${PROJECT_NAME}
              GTest::gtest)

  add_executable(${PROJECT_NAME}_test)
  target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_test_base)
endif() # ${PROJECT_NAME}_BUILD_TESTING OR PROJECT_IS_TOP_LEVEL
