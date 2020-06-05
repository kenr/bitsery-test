cmake_minimum_required(VERSION 3.16)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(bitsery-test VERSION 0.0.0 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(spdlog CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(test_sdfu
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_sdfu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdfu_codec.cpp
)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

target_compile_features(test_sdfu PRIVATE cxx_variadic_templates)

target_link_libraries(test_sdfu
    PRIVATE
        spdlog::spdlog
        Threads::Threads
        Catch2::Catch2
)

set_target_properties(test_sdfu PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS ON)

#target_include_directories(test_sdfu PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/include
#    ${CMAKE_CURRENT_SOURCE_DIR}/include/implementation
#)

