add_executable(a_star src/main.cpp src/graph.cpp)
target_include_directories(a_star PUBLIC include)

target_link_libraries(a_star PRIVATE
  comm_lib
  raptor_lib
)