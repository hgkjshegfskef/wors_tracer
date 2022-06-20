include_guard()

cmake_minimum_required(VERSION 3.22)

# ~~~
# Sets the list of allowed build types. For single config generators sets the first build type as
# default.
#
# NOTE
#   Build type flags can by configured via:
#       - CMAKE_<LANG>_FLAGS_<CONFIG>
#       - CMAKE_<EXE|SHARED|STATIC|MODULE>_LINKER_FLAGS_<CONFIG>
#   where <CONFIG> is one of the build types.
#
#   When referring to the build type in code, use $<CONFIG> and $<CONFIG:...> generator expressions.
#
# EXAMPLE
#   util_set_allowed_build_types(BUILD_TYPES Debug Release RelWithDebInfo MyBuildType)
#
macro(util_set_allowed_build_types)
    list(APPEND CMAKE_MESSAGE_CONTEXT "util_set_allowed_build_types")

    set(flags)
    set(args)
    set(list_args BUILD_TYPES)
    cmake_parse_arguments(arg "${flags}" "${args}" "${list_args}" ${ARGN})

    foreach(argument IN ITEMS BUILD_TYPES)
        if(NOT arg_${argument})
            message(FATAL_ERROR "${argument} is a required argument")
        endif()
        if(${argument} IN_LIST arg_KEYWORDS_MISSING_VALUES)
            message(FATAL_ERROR "${argument} requires at least one value")
        endif()
    endforeach()

    get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(isMultiConfig)
        set(CMAKE_CONFIGURATION_TYPES "${arg_BUILD_TYPES}")
    else()
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${arg_BUILD_TYPES}")
        if(CMAKE_BUILD_TYPE)
            if(NOT (CMAKE_BUILD_TYPE IN_LIST arg_BUILD_TYPES))
                message(FATAL_ERROR "Build type '${CMAKE_BUILD_TYPE}' is not supported.")
            endif()
        else()
            list(GET arg_BUILD_TYPES 0 firstBuildType)
            set(CMAKE_BUILD_TYPE
                ${firstBuildType}
                CACHE STRING "" FORCE)
        endif()
    endif()

    list(POP_BACK CMAKE_MESSAGE_CONTEXT)
endmacro()

# ~~~
# Check if compiler flag is supported. The result is stored in internal cache entry
# ${PROJECT_NAME}_SUPPORTS_FLAG${flag}, where ${flag} is the same as the supplied argument FLAG,
# with all '-', '/', '=', ',' replaced by '_'.
#
# NOTE
#   Additional CMake variables can be set before calling the function. See [1] for the list of variables.
#
# EXAMPLE
#   project(foo)
#   # ...
#   set(CMAKE_REQUIRED_LINK_OPTIONS -fsanitize=address,undefined)
#   util_check_cxx_flag(FLAG -fsanitize=address,undefined)
#   unset(CMAKE_REQUIRED_LINK_OPTIONS)
#   # result is stored in foo_SUPPORTS_FLAG_fsanitize_address_undefined
#
# [1] https://cmake.org/cmake/help/latest/module/CheckCXXSourceCompiles.html#command:check_cxx_source_compiles
#
macro(util_check_cxx_flag)
    list(APPEND CMAKE_MESSAGE_CONTEXT "util_check_cxx_flag")

    set(flags)
    set(args FLAG)
    set(list_args)
    cmake_parse_arguments(arg "${flags}" "${args}" "${list_args}" ${ARGN})

    foreach(argument IN ITEMS FLAG)
        if(NOT arg_${argument})
            message(FATAL_ERROR "${argument} is a required argument")
        endif()
        if(${argument} IN_LIST arg_KEYWORDS_MISSING_VALUES)
            message(FATAL_ERROR "${argument} requires at least one value")
        endif()
    endforeach()

    if(CMAKE_CXX_FLAGS)
        message(WARNING "util_check_cxx_flag() may give a false positive because CMAKE_CXX_FLAGS"
                        " is set. Please don't use CMAKE_CXX_FLAGS.")
    endif()

    # Replace all '-', '/', '=', ',' with '_'.
    string(REGEX REPLACE [=[[-/=,]+]=] _ flagEscaped ${arg_FLAG})

    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(${arg_FLAG} ${PROJECT_NAME}_SUPPORTS_FLAG${flagEscaped})

    list(POP_BACK CMAKE_MESSAGE_CONTEXT)
endmacro()

macro(util_reset_cxx_flags_for)
    list(APPEND CMAKE_MESSAGE_CONTEXT "util_reset_cxx_flags_for")

    set(flags)
    set(args)
    set(list_args BUILD_TYPES)
    cmake_parse_arguments(arg "${flags}" "${args}" "${list_args}" ${ARGN})

    foreach(argument IN ITEMS BUILD_TYPES)
        if(NOT arg_${argument})
            message(FATAL_ERROR "${argument} is a required argument")
        endif()
        if(${argument} IN_LIST arg_KEYWORDS_MISSING_VALUES)
            message(FATAL_ERROR "${argument} requires at least one value")
        endif()
    endforeach()

    foreach(build_type IN LISTS arg_BUILD_TYPES)
        string(TOUPPER ${build_type} toupper_build_type)
        set(CMAKE_CXX_FLAGS_${toupper_build_type} "")
    endforeach()

    list(POP_BACK CMAKE_MESSAGE_CONTEXT)
endmacro()

macro(util_install_export_project_name_target)
    list(APPEND CMAKE_MESSAGE_CONTEXT "util_install_export_project_name_target")

    include(GNUInstallDirs)
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

    install(
        EXPORT ${PROJECT_NAME}Targets
        NAMESPACE ${PROJECT_NAME}::
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/Config.cmake.in" "@PACKAGE_INIT@
include(\"\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake\")")

    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        "${CMAKE_CURRENT_BINARY_DIR}/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
        NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO)

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION "${${PROJECT_NAME}_VERSION}"
        COMPATIBILITY AnyNewerVersion)

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

    export(EXPORT ${PROJECT_NAME}Targets
           FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

    list(POP_BACK CMAKE_MESSAGE_CONTEXT)
endmacro()

macro(util_fetch_content_git)
    list(APPEND CMAKE_MESSAGE_CONTEXT "util_fetch_content_git")

    set(flags QUIET)
    set(args NAME REPOSITORY COMMIT PATCH)
    set(list_args BUILD_TYPES)
    cmake_parse_arguments(arg "${flags}" "${args}" "${list_args}" ${ARGN})

    foreach(argument IN ITEMS NAME REPOSITORY COMMIT)
        if(NOT arg_${argument})
            message(FATAL_ERROR "${argument} is a required argument")
        endif()
        if(${argument} IN_LIST arg_KEYWORDS_MISSING_VALUES)
            message(FATAL_ERROR "${argument} requires at least one value")
        endif()
    endforeach()

    if(NOT ${PROJECT_NAME}_GIT_PROGRAM)
        find_program(${PROJECT_NAME}_GIT_PROGRAM git REQUIRED)

        execute_process(
            COMMAND ${${PROJECT_NAME}_GIT_PROGRAM} --version
            RESULT_VARIABLE result
            OUTPUT_VARIABLE gitVersionOutput
            ERROR_VARIABLE gitVersionOutput)

        if(result)
            message(FATAL_ERROR "'${${PROJECT_NAME}_GIT_PROGRAM} --version' "
                                "failed with output: ${gitVersionOutput}")
        endif()

        string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" gitVersion ${gitVersionOutput})
        message(STATUS "Using git ${gitVersion}")
    endif()

    include(FetchContent)
    FetchContent_Declare(
        ${arg_NAME}
        ${arg_QUIET}
        GIT_REPOSITORY ${arg_REPOSITORY}
        GIT_TAG ${arg_COMMIT}
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        GIT_CONFIG "advice.detachedHead=false"
        PATCH_COMMAND ${${PROJECT_NAME}_GIT_PROGRAM} apply ${arg_PATCH}
        USES_TERMINAL_DOWNLOAD ON
        USES_TERMINAL_UPDATE ON
        USES_TERMINAL_PATCH ON
        USES_TERMINAL_CONFIGURE ON
        USES_TERMINAL_BUILD ON)

    FetchContent_MakeAvailable(${arg_NAME})

    list(POP_BACK CMAKE_MESSAGE_CONTEXT)
endmacro()
