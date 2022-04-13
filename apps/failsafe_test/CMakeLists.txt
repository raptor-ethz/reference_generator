add_executable(failsafe_test src/main.cpp)

target_link_libraries(failsafe_test PUBLIC
  raptor_lib
  comm_lib
)