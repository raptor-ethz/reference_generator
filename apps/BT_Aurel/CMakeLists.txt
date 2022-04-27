add_executable(BT_Aurel src/main.cpp)

target_link_libraries(BT_Aurel PUBLIC
  raptor_lib
  comm_lib
)