add_executable(gripper_test src/main.cpp)

target_link_libraries(gripper_test PRIVATE
raptor_lib
)