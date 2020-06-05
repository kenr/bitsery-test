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
set(CMAKE_CXX_EXTENSIONS OFF) # For g++ this ensures -std=c++11 and not -std=gnu++11

target_compile_features(${NRF_DEVICE_LIB} PRIVATE cxx_variadic_templates)

add_executable(test_sdfu
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_sdfu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdfu/sdfu_codec.cpp
)

target_link_libraries(test_sdfu
    PRIVATE
        spdlog::spdlog
        Threads::Threads
        Catch2::Catch2
        ${NRF_DEVICE_LIB}
        ${${NRF_DEVICE_LIB}_PLATFORM_DEP_LIBS}
)

target_include_directories(test_sdfu PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/implementation
)

add_executable(test_device_enumeration
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_log_message.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_device_enumeration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_device_lister.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_device_pool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_usb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_usb_device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_jlink.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_serialport.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device_lister.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device_list.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/async_worker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/event_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/log_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/usb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/usb_device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/usb_lister.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jlink/jlink.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jlink/jlink_lister.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jlink/jlink_device_setup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jlink/universal_platform/jlink_hw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/serialport/serialport.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/serialport/serialport_lister.cpp
)

target_link_libraries(test_device_enumeration
    PRIVATE
        spdlog::spdlog
        Threads::Threads
        Catch2::Catch2
        ${NRF_DEVICE_LIB}
        ${${NRF_DEVICE_LIB}_PLATFORM_DEP_LIBS}
)

target_include_directories(test_device_enumeration PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/implementation
)

if(NOT IGNORE_CLANG_FORMAT)
    target_clangformat_check_setup(${NRF_DEVICE_LIB} test_device_enumeration test_sdfu)
else()
    message(STATUS "IGNORE_CLANG_FORMAT is ON. Ignoring clang-format.")
endif()


# CLI for device lister
find_package(CLI11 CONFIG REQUIRED)

add_executable(nrfc
    cli/main.cpp
)

target_link_libraries(nrfc
    PRIVATE
        CLI11::CLI11
        spdlog::spdlog
        Threads::Threads
        ${NRF_DEVICE_LIB}
        ${${NRF_DEVICE_LIB}_PLATFORM_DEP_LIBS}
)

target_include_directories(nrfc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(TARGETS nrfc
    RUNTIME DESTINATION bin
)

install(TARGETS test_device_enumeration
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
)

install(TARGETS test_sdfu
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
)

install(TARGETS ${NRF_DEVICE_LIB}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
)

# For Windows: Find the fmt DLL in vcpkg and install it.
if(WIN32)
    string(REPLACE "\\" "/" VCPKG_ROOT_FWD $ENV{VCPKG_ROOT})
    if("${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}" MATCHES "x64")
        set(VCPKG_TRIPLET "x64-windows")
    elseif("${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}" MATCHES "X86")
        set(VCPKG_TRIPLET "x86-windows")
    else()
        message(WARNING "Could not find generator platform on Windows. Defaulting to x86. Was: ${CMAKE_GENERATOR_PLATFORM}")
        set(VCPKG_TRIPLET "x86-windows")
    endif()

    set(BINARY_SOURCE_PATH "${VCPKG_ROOT_FWD}/installed/${VCPKG_TRIPLET}")

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(BINARY_SOURCE_PATH "${BINARY_SOURCE_PATH}/debug/bin")
    else()
        set(BINARY_SOURCE_PATH "${BINARY_SOURCE_PATH}/bin")
    endif()

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        install(FILES "${BINARY_SOURCE_PATH}/fmtd.dll" DESTINATION bin)
    else()
        install(FILES "${BINARY_SOURCE_PATH}/fmt.dll" DESTINATION bin)
    endif()
endif()

# Copy shared libraries that are linked programatically at runtime
if(NOT WIN32)
    find_library(JLINKARM_NRF51_NRFJPROGDLL jlinkarm_nrf51_nrfjprogdll PATHS ${NRFJPROG_PATH})
    find_library(JLINKARM_NRF52_NRFJPROGDLL jlinkarm_nrf52_nrfjprogdll PATHS ${NRFJPROG_PATH})
    find_library(JLINKARM_NRF53_NRFJPROGDLL jlinkarm_nrf53_nrfjprogdll PATHS ${NRFJPROG_PATH})
    find_library(JLINKARM_NRF91_NRFJPROGDLL jlinkarm_nrf91_nrfjprogdll PATHS ${NRFJPROG_PATH})
    find_library(JLINKARM_UNKNOWN_NRFJPROGDLL jlinkarm_unknown_nrfjprogdll PATHS ${NRFJPROG_PATH})
    find_library(NRFDFU nrfdfu PATHS ${NRFJPROG_PATH})
    find_library(NRFJPROGDLL nrfjprogdll PATHS ${NRFJPROG_PATH})

    set(NRFJPROG_INDIRECT ${JLINKARM_NRF51_NRFJPROGDLL} ${JLINKARM_NRF52_NRFJPROGDLL} ${JLINKARM_NRF53_NRFJPROGDLL} ${JLINKARM_NRF91_NRFJPROGDLL} ${JLINKARM_UNKNOWN_NRFJPROGDLL} ${NRFDFU} ${NRFJPROGDLL})

    install(FILES ${NRFJPROG_HIGHLEVEL} ${NRFJPROG_INDIRECT} DESTINATION bin)
else()
    file(GLOB JLINK_NRFXX_NRFJPROGDLL "${NRFJPROG_PATH}/jlinkarm_*.dll")
    set(NRFJPROG_HIGHLEVEL_DLL "${NRFJPROG_PATH}/highlevelnrfjprog.dll")
    set(NRFDFU "${NRFJPROG_PATH}/nrfdfu.dll")
    set(NRFJPROGDLL "${NRFJPROG_PATH}/nrfjprog.dll")

    set(NRFJPROG_INDIRECT ${JLINK_NRFXX_NRFJPROGDLL} ${NRFDFU} ${NRFJPROGDLL})

    install(FILES ${NRFJPROG_HIGHLEVEL_DLL} ${NRFJPROG_INDIRECT} DESTINATION bin)
endif()
