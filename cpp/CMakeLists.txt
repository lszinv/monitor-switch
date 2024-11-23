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

# Enable static linking
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
  include
  third_party/include
)

set(SOURCES
  src/ConfigManager.cpp
  src/DisplayDevice.cpp
  src/DisplayDeviceManager.cpp
  src/main.cpp
)

# Add executable
add_executable(MonitorVCPFeatures ${SOURCES})

# Link required Windows libraries
target_link_libraries(MonitorVCPFeatures
  PRIVATE
  dxva2
  # For SetupAPI
  setupapi
  cfgmgr32
  )


# Set Windows subsystem
set_target_properties(MonitorVCPFeatures PROPERTIES
    WIN32_EXECUTABLE FALSE    # Console application
)
