add_executable(rollout_demo src/main.cpp)

target_link_libraries(rollout_demo PUBLIC
  raptor_lib
  comm_lib
)