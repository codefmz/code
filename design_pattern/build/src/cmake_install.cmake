# Install script for directory: E:/Code/computer_base/design_pattern/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/degin_pattern")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("E:/Code/computer_base/design_pattern/build/src/simple_factory/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/strategy/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/proxy/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/decorate/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/factory/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/prototype/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/template/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/facade/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/builder/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/observer/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/state/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/memento/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/component/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/singleton/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/bridge/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/command/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/handle/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/mediator/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/flywight/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/interpreter/cmake_install.cmake")
  include("E:/Code/computer_base/design_pattern/build/src/visit/cmake_install.cmake")

endif()

