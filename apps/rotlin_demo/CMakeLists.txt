add_executable(rotlin_demo src/main.cpp)

target_link_libraries(rotlin_demo PUBLIC
  raptor_lib
  comm_lib
)