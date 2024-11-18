cmake_minimum_required(VERSION 3.5)
project(gain_sch)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()



# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)


# Add your executable
add_executable(gain_schedulling src/gain_schedulling.cpp)
add_executable(binary_search src/binary_search.cpp)


# Link against ROS 2 dependencies
ament_target_dependencies(gain_schedulling rclcpp sensor_msgs geometry_msgs)
ament_target_dependencies(binary_search rclcpp sensor_msgs)


# Install
install(TARGETS gain_schedulling
  binary_search
  DESTINATION lib/${PROJECT_NAME})

ament_package()
