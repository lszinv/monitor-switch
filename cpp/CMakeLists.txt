cmake_minimum_required(VERSION 3.10)

# Set up cross-compilation
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc-posix)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++-posix)
set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Set paths
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

project(MonitorVCPFeatures)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Properties
set(THREADS_PREFER_PTHREAD_FLAG ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
  endif()

include_directories(
  include
  third_party/include
  /usr/x86_64-w64-mingw32/include
  /usr/lib/gcc/x86_64-w64-mingw32/*/include/c++
  /usr/lib/gcc/x86_64-w64-mingw32/*/include/c++/x86_64-w64-mingw32
)

set(SOURCES
  src/DisplayDevice.cpp
  src/DisplayDeviceManager.cpp
  src/DisplayDeviceId.cpp
  src/MonitorSwitchCLI.cpp
)

# Add Libraries
add_library(MonitorSwitch SHARED ${SOURCES})
add_library(MonitorSwitchCLILib STATIC ${SOURCES})

# Add executables
add_executable(MonitorSwitchCLI src/main.cpp)
set_target_properties(MonitorSwitchCLI PROPERTIES
    LINK_FLAGS "-static -static-libgcc -static-libstdc++"
  )
add_executable(DedicatedMonitorSwitch src/DedicatedMain.cpp)
set_target_properties(DedicatedMonitorSwitch PROPERTIES
    LINK_FLAGS "-static -static-libgcc -static-libstdc++"
  )

# Link required Windows libraries
target_link_libraries(MonitorSwitch
  PRIVATE
  dxva2
  # For SetupAPI
  setupapi
  cfgmgr32
)


target_link_libraries(MonitorSwitchCLILib
  PRIVATE
  dxva2
  # For SetupAPI
  setupapi
  cfgmgr32
  )

target_link_libraries(MonitorSwitchCLI MonitorSwitchCLILib)
target_link_libraries(DedicatedMonitorSwitch MonitorSwitchCLILib)

# Set Windows subsystem
set_target_properties(MonitorSwitchCLI PROPERTIES
    WIN32_EXECUTABLE FALSE # CLI App
)
