cmake_minimum_required(VERSION 3.8)
project(service_new)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)


#...
find_package(tutorial_interfaces REQUIRED)        # CHANGE
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)


add_executable(server_a src/server_a.cpp)
ament_target_dependencies(server_a
  rclcpp tutorial_interfaces)                      #CHANGE
add_executable(server_b src/server_b.cpp)
ament_target_dependencies(server_b
    rclcpp tutorial_interfaces)  

add_executable(client src/client.cpp)
ament_target_dependencies(client
  rclcpp tutorial_interfaces)                      #CHANGE

install(TARGETS
  server_a
  server_b
  client
  DESTINATION lib/${PROJECT_NAME})

ament_package()