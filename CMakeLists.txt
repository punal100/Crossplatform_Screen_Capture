# -----Start-----#
cmake_minimum_required(VERSION 3.0.0)
set(CPSC_BP_PROJECT_NAME Crossplatform_Screen_Capture)
project(${CPSC_BP_PROJECT_NAME} VERSION 1.0.0)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo" CACHE STRING "Supported configs" FORCE)

set(CPSC_BP_SOURCE_DIR
    src
)
set(CPSC_BP_HEADERS_DIR
    include
)
set(CPSC_BP_HEADERS
    ${CPSC_BP_HEADERS_DIR}/*.h
)
set(CPSC_BP_SOURCE
    ${CPSC_BP_SOURCE_DIR}/main.cpp
)
set(CPSC_BP_LIBRARIES_DIR
    libs
)

# set(CPSC_BP_LIBRARIES
# Lib_A
# Lib_B
# )

# -----Build Location----#
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/bin)

# ########
# GTest #
# ########
# enable_testing()
add_subdirectory(libs/gtest-1.7.0)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# ##########
# Project #
# ##########
add_executable(${CPSC_BP_PROJECT_NAME} ${CPSC_BP_SOURCE})
target_include_directories(${CPSC_BP_PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# foreach(LIBRARY ${CPSC_BP_LIBRARIES})
# add_subdirectory("${CPSC_BP_LIBRARIES_DIR}/${LIBRARY}")
# endforeach(LIBRARY)
# target_link_libraries(${CPSC_BP_PROJECT_NAME} ${CPSC_BP_LIBRARIES})