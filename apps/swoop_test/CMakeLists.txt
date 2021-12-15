add_executable(swoop_test main.cpp)

target_link_libraries(swoop_test PRIVATE
raptor_lib
comm_lib
)