add_executable(grab_object src/main.cpp)

target_link_libraries(grab_object PRIVATE
comm_lib
raptor_lib
)