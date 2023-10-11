cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -g -Wall -Wextra -pedantic \
                -Wno-unused-result -Wparentheses -Wsign-compare")

project(WaveShareHAT VERSION 1.0 LANGUAGES C)

# Option to build tests
option(BUILD_TESTS "Build tests" OFF)

# Check if Unity is installed
find_package(Unity QUIET)

if (BUILD_TESTS)
    if (Unity_FOUND)
        message(STATUS "Unity found, building tests.")
    else()
        message(STATUS "Unity not found, attempting to install.")
        include(ExternalProject)

        # You can set the Unity version and download URL accordingly
        ExternalProject_Add(
            unity
            PREFIX ${CMAKE_BINARY_DIR}/_deps
            GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
            GIT_TAG master
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        )

        # Add a target to build and install Unity
        add_custom_target(UnityInstall
            DEPENDS unity
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/_deps/unity --target install
        )

        set_target_properties(unity PROPERTIES EXCLUDE_FROM_ALL TRUE)

        message(STATUS "Unity installed, building tests.")
    endif()

    add_executable(WaveShareHAT
        src/util.c
        waveshare.c
        test/test_util.c  # Add the test file to the build
    )

    target_link_libraries(WaveShareHAT Unity)  # Link with Unity for testing

    # Make sure Unity is installed before building the tests
    add_dependencies(WaveShareHAT UnityInstall)

else()
    add_executable(WaveShareHAT
        src/util.c
        waveshare.c
    )
endif()

