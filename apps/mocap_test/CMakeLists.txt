add_executable(mocap_test src/main.cpp)

target_link_libraries(mocap_test PRIVATE
comm_lib
raptor_lib
)