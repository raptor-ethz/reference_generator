add_executable(gripper_takeoff src/main.cpp)

target_link_libraries(gripper_takeoff PRIVATE
comm_lib
raptor_lib
)