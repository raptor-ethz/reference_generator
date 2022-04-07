add_executable(linear_gripper src/main.cpp)

target_link_libraries(linear_gripper PRIVATE
comm_lib
raptor_lib
)