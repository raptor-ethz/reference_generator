add_executable(_TEMPLATE src/main.cpp)

target_link_libraries(_TEMPLATE PUBLIC
  raptor_lib
  comm_lib
)