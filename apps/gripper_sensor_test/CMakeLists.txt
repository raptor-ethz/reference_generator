add_executable(gripper_sensor_test src/main.cpp)

target_link_libraries(gripper_sensor_test PRIVATE
comm_lib
raptor_lib
)