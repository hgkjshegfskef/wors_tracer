cmake_minimum_required(VERSION 3.22)

project(wors_tracer LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

  set(CMAKE_CXX_EXTENSIONS OFF)

  set(buildTypes Debug Release RelWithDebInfo Testing)

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
  set(isGNU $<CXX_COMPILER_ID:GNU>)
  set(isClang $<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>)
  set(isGNUorClang $<OR:${isGNU},${isClang}>)

  target_compile_options(
    ${PROJECT_NAME}_common_flags
    INTERFACE
      $<${isGNUorClang}:-Wall>
      $<${isGNUorClang}:-Wextra>
      $<${isDebug}:$<${isGNUorClang}:$<IF:${hasOg},-Og,-g>>>
      $<${isDebug}:$<${isGNUorClang}:-ggdb3>>
      $<${isRelease}:$<${isGNUorClang}:-O3>>
      $<$<OR:${isRelWithDebInfo},${isTesting}>:$<${isGNUorClang}:-O2>>
      $<$<OR:${isRelWithDebInfo},${isTesting}>:$<${isGNUorClang}:-g>>
      $<${isTesting}:$<${isGNUorClang}:$<${hasAsanUbsan}:-fsanitize=address,undefined>>>
      $<${isGNU}:-fdiagnostics-color=always>
      $<${isClang}:-fcolor-diagnostics>)

  target_compile_definitions(
    ${PROJECT_NAME}_common_flags
    INTERFACE $<${isRelease}:$<${isGNUorClang}:NDEBUG>>)

  target_link_options(
    ${PROJECT_NAME}_common_flags
    INTERFACE
    $<$<AND:$<CONFIG:Testing>,${isGNUorClang},${hasAsanUbsan}>:-fsanitize=address,undefined>
  )
endif()

include(FindPackageMessage)
find_package(fmt CONFIG REQUIRED)
find_package_message(fmt "Using fmt ${fmt_VERSION}" "[${fmt_VERSION}]")
find_package(spdlog CONFIG REQUIRED)
find_package_message(spdlog "Using spdlog ${spdlog_VERSION}"
                     "[${spdlog_VERSION}]")

add_executable(${PROJECT_NAME} main.cc log.cc vec3.cc)
target_compile_definitions(
  ${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL
                          SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE $<TARGET_NAME_IF_EXISTS:${PROJECT_NAME}_common_flags>
                          spdlog::spdlog fmt::fmt)

if(${PROJECT_NAME}_BUILD_TESTING OR PROJECT_IS_TOP_LEVEL)
  enable_testing()

  find_package(GTest CONFIG REQUIRED)
  find_package_message(GTest "Using GTest ${GTest_VERSION}"
                       "[${GTest_VERSION}]")

  add_executable(${PROJECT_NAME}_test vec3_test.cc)

  target_link_libraries(
    ${PROJECT_NAME}_test
    PRIVATE GTest::gtest GTest::gtest_main
            $<TARGET_NAME_IF_EXISTS:${PROJECT_NAME}_common_flags>)
endif()
