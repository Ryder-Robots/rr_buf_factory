# find_package(rtest REQUIRED)
find_package(ament_cmake_gmock REQUIRED)

ament_add_gmock(${PROJECT_NAME}_state_maintainer_test
    ${CMAKE_SOURCE_DIR}/src/rr_state_maintainer.cpp
    rr_state_maintainer_test.cpp
)

target_link_libraries(${PROJECT_NAME}_state_maintainer_test ${PROJECT_NAME}_lib)