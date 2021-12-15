add_executable(gripper_test src/main.cpp)

target_link_libraries(gripper_test PRIVATE
comm_lib
raptor_lib
)