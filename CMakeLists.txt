cmake_minimum_required(VERSION 3.8)
project(rr_buf_factory)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp  REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(rr_interfaces REQUIRED)

# Project library binaries
# add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}_lib
  src/rr_state_maintainer.cpp
)
ament_target_dependencies(${PROJECT_NAME}_lib rclcpp rr_interfaces std_msgs sensor_msgs builtin_interfaces)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)


target_link_libraries(${PROJECT_NAME}_lib)

# Install library and export targets
install(TARGETS 
  ${PROJECT_NAME}_lib
  DESTINATION lib/${PROJECT_NAME}
)

# Install headers
install(DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)

# Export old-style CMake variables
ament_export_include_directories(
  include
)

if(BUILD_TESTING)
  add_subdirectory(test)
  # find_package(ament_cmake_gtest REQUIRED)
  # ament_add_gtest(${PROJECT_NAME}_test test/rr_controller_test.cpp)
  # target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}_lib)
  # ament_target_dependencies(${PROJECT_NAME}_test rclcpp rr_interfaces sensor_msgs)
endif()

ament_package()
