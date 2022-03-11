add_executable(mocap_fail_test src/main.cpp)

target_link_libraries(mocap_fail_test PUBLIC
  raptor_lib
  comm_lib
)