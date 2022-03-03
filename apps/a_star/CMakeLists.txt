add_executable(a_star src/main.cpp)

target_link_libraries(a_star PRIVATE
comm_lib
raptor_lib
)