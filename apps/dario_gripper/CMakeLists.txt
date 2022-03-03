add_executable(dario_gripper src/main.cpp)

target_link_libraries(dario_gripper PRIVATE
comm_lib
raptor_lib
)