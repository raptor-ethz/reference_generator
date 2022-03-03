add_executable(px4_cmd_test src/main.cpp)

target_link_libraries(px4_cmd_test PRIVATE
comm_lib
raptor_lib
)