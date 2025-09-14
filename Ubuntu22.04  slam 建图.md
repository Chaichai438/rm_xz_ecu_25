

Ubuntu22.04  slam 建图

先换源（arm64 Ubuntu22.04）

```bash
sudo nano /etc/apt/sources.list
# 默认注释了源码镜像以提高 apt update 速度，如有需要可自行取消注释
# 默认注释了源码镜像以提高 apt update 速度，如有需要可自行取消注释
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ focal main restricted universe multiverse
# deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ focal main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ focal-updates main restricted universe multiverse
# deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ focal-updates main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ focal-backports main restricted universe multiverse
# deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ focal-backports main restricted universe multiverse

# 以下安全更新软件源包含了官方源与镜像站配置，如有需要可自行修改注释切换
deb http://ports.ubuntu.com/ubuntu-ports/ focal-security main restricted universe multiverse
# deb-src http://ports.ubuntu.com/ubuntu-ports/ focal-security main restricted universe multiverse

# 预发布软件源，不建议启用
# deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ focal-proposed main restricted universe multiverse
# # deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ focal-proposed main restricted universe multiverse
```

ctrl+o 保存  ctrl+x退出


```c
sudo apt-get update
sudo apt-get upgrade
   
```

安装桌面

```bash
sudo apt update 
sudo apt upgrade -y
sudo apt install xfce4 xfce4-goodies -y

sudo apt install xrdq -y
sudo systemctl enable xrdp
sudo systemctl start xrdp
sudo systemctl status xrdp
/etc/xrdp/startwm.sh
#!/bin/sh
unset DBUS_SESSION_BUS_ADDRESS
unset XDG_RUNTIME_DIR
exec startxfce4
sudo adduser xrdp ssl-cert
sudo systemctl restart xrdp
sudo netstat -tuln | grep 3389
sudo ufw allow 3389
sudo ufw reload
```

安装ros2

配置环境

```bash
sudo apt-get install g++
sudo apt-get install python3.6
sudo apt update
sudo apt upgrade
source /opt/ros/humble/setup.bash
```

cartographer编译报错CMake Error in CMakeLists.txt:  Imported target "Boost::iostreams" includes non-existent path     "/include"

1，进入cartographer-ws/src/cartographer/cmake.list中

这一段

```
set(BOOST_COMPONENTS iostreams)
if(WIN32)
  list(APPEND BOOST_COMPONENTS zlib)
  set(Boost_USE_STATIC_LIBS FALSE)
endif()
find_package(Boost REQUIRED COMPONENTS ${BOOST_COMPONENTS})
```

改成

```
set(BOOST_COMPONENTS iostreams)
if(WIN32)
  list(APPEND BOOST_COMPONENTS zlib)
  set(Boost_USE_STATIC_LIBS FALSE)
endif()

# 优先查找标准路径 /usr/include，避免意外指向 /include
set(Boost_NO_SYSTEM_PATHS OFF)
set(Boost_NO_BOOST_CMAKE ON)  # 避免使用可能有问题的 boost-cmake 包

find_package(Boost REQUIRED COMPONENTS ${BOOST_COMPONENTS})

# 手动检查 Boost_INCLUDE_DIRS 是否为 "/include"，如果是就修复
if("${Boost_INCLUDE_DIRS}" STREQUAL "/include")
  message(WARNING "Boost_INCLUDE_DIRS incorrectly set to /include, overriding to /usr/include")
  set(Boost_INCLUDE_DIRS "/usr/include")
endif()

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
```



2，进入cartographer_ws/src/cartographer_ros/cartographer_ros/cmake.list中

直接复制我的这段粘贴上去

```
# Copyright 2016 The Cartographer Authors
# Licensed under the Apache License, Version 2.0

cmake_minimum_required(VERSION 3.0.2)
project(cartographer_ros)

# -------------------- Package Dependencies --------------------
set(PACKAGE_DEPENDENCIES
  cartographer_ros_msgs
  geometry_msgs
  message_runtime
  nav_msgs
  pcl_conversions
  rosbag
  roscpp
  roslib
  sensor_msgs
  std_msgs
  tf2
  tf2_eigen
  tf2_ros
  urdf
  visualization_msgs)

# -------------------- Boost --------------------
if(WIN32)
  set(Boost_USE_STATIC_LIBS FALSE)
endif()
find_package(Boost REQUIRED COMPONENTS system iostreams)

# -------------------- PCL --------------------
find_package(PCL REQUIRED COMPONENTS common)

# -------------------- Cartographer --------------------
find_package(cartographer REQUIRED)
include("${CARTOGRAPHER_CMAKE_DIR}/functions.cmake")
option(BUILD_GRPC "build features that require Cartographer gRPC support" OFF)
google_initialize_cartographer_project()
google_enable_testing()
set(CARTOGRAPHER_GMOCK_LIBRARIES ${GMOCK_LIBRARIES})

# -------------------- Catkin --------------------
find_package(catkin REQUIRED COMPONENTS ${PACKAGE_DEPENDENCIES})

# -------------------- Additional Dependencies --------------------
include(FindPkgConfig)
find_package(absl REQUIRED)
find_package(LuaGoogle REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(urdfdom_headers REQUIRED)

if(DEFINED urdfdom_headers_VERSION)
  if(${urdfdom_headers_VERSION} GREATER 0.4.1)
    add_definitions(-DURDFDOM_HEADERS_HAS_SHARED_PTR_DEFS)
  endif()
endif()

include_directories(${urdfdom_headers_INCLUDE_DIRS})

# -------------------- Override GTest for GMock --------------------
set(GTEST_FOUND TRUE)
set(GTEST_INCLUDE_DIRS ${GMOCK_INCLUDE_DIRS})
set(GTEST_LIBRARIES ${CARTOGRAPHER_GMOCK_LIBRARIES})

catkin_package(
  CATKIN_DEPENDS ${PACKAGE_DEPENDENCIES}
  DEPENDS PCL EIGEN3 Boost urdfdom_headers
  INCLUDE_DIRS .
  LIBRARIES ${PROJECT_NAME})

# -------------------- Source Collection --------------------
file(GLOB_RECURSE ALL_SRCS "cartographer_ros/*.cc" "cartographer_ros/*.h")
file(GLOB_RECURSE ALL_TESTS "cartographer_ros/*_test.cc")
file(GLOB_RECURSE ALL_EXECUTABLES "cartographer_ros/*_main.cc")
file(GLOB_RECURSE ALL_GRPC_FILES "cartographer_ros/cartographer_grpc/*")

list(REMOVE_ITEM ALL_SRCS ${ALL_TESTS})
list(REMOVE_ITEM ALL_SRCS ${ALL_EXECUTABLES})
if(NOT ${BUILD_GRPC})
  list(REMOVE_ITEM ALL_SRCS ${ALL_GRPC_FILES})
  list(REMOVE_ITEM ALL_TESTS ${ALL_GRPC_FILES})
  list(REMOVE_ITEM ALL_EXECUTABLES ${ALL_GRPC_FILES})
endif()

add_library(${PROJECT_NAME} STATIC ${ALL_SRCS})
add_subdirectory("cartographer_ros")

target_link_libraries(${PROJECT_NAME} PUBLIC cartographer)

# -------------------- Includes --------------------
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
  ${LUA_INCLUDE_DIR}
  ${PCL_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${PCL_LIBRARIES}
  ${Boost_LIBRARIES}
  ${catkin_LIBRARIES})

add_dependencies(${PROJECT_NAME} ${catkin_EXPORTED_TARGETS})

# Binary + Source include
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>)

# -------------------- Compiler Flags --------------------
set(BLACKLISTED_PCL_DEFINITIONS " -march=native -msse4.2 -mfpmath=sse ")
foreach(DEFINITION ${PCL_DEFINITIONS})
  list(FIND BLACKLISTED_PCL_DEFINITIONS "${DEFINITION}" DEFINITIONS_INDEX)
  if(${DEFINITIONS_INDEX} GREATER -1)
    continue()
  endif()
  set(TARGET_COMPILE_FLAGS "${TARGET_COMPILE_FLAGS} ${DEFINITION}")
endforeach()

set(TARGET_COMPILE_FLAGS "${TARGET_COMPILE_FLAGS} ${GOOG_CXX_FLAGS}")
set_target_properties(${PROJECT_NAME} PROPERTIES
  COMPILE_FLAGS "${TARGET_COMPILE_FLAGS}")

# -------------------- Testing --------------------
if(CATKIN_ENABLE_TESTING)
  foreach(TEST_SOURCE_FILENAME ${ALL_TESTS})
    get_filename_component(TEST_NAME ${TEST_SOURCE_FILENAME} NAME_WE)
    catkin_add_gtest(${TEST_NAME} ${TEST_SOURCE_FILENAME})
    target_link_libraries(${TEST_NAME} ${GMOCK_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
    target_include_directories(${TEST_NAME} SYSTEM PUBLIC
      ${LUA_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})
    target_link_libraries(${TEST_NAME}
      ${LUA_LIBRARIES} ${catkin_LIBRARIES} cartographer ${PROJECT_NAME})
    add_dependencies(${TEST_NAME} ${catkin_EXPORTED_TARGETS})
    set_target_properties(${TEST_NAME} PROPERTIES COMPILE_FLAGS ${TARGET_COMPILE_FLAGS})
    if(WIN32)
      target_compile_definitions(${TEST_NAME} PUBLIC -DGTEST_LINKED_AS_SHARED_LIBRARY)
    endif()
  endforeach()
endif()

# -------------------- Install --------------------
install(DIRECTORY launch urdf configuration_files
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

install(PROGRAMS scripts/tf_remove_frames.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION})

file(GLOB_RECURSE HDRS "cartographer_ros/*.h")
foreach(HDR ${HDRS})
  file(RELATIVE_PATH REL_FIL ${PROJECT_SOURCE_DIR} ${HDR})
  get_filename_component(INSTALL_DIR ${REL_FIL} DIRECTORY)
  install(FILES ${HDR} DESTINATION include/${INSTALL_DIR})
endforeach()

```

```

```

启动

source devel/setup.bash
cd ros1_ws
roslaunch rplidar_ros rplidar_a1.launch

---------cartographer 
cd cartographer_ws
roslaunch cartographer_ros demo_revo_lds.launch
source install_isolated/setup.bash![Snipaste_2025-08-05_13-15-50](C:\Users\86152\Desktop\Snipaste_2025-08-05_13-15-50.jpg)
