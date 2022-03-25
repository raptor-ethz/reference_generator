add_executable(grasp_video src/main.cpp)

target_link_libraries(grasp_video PRIVATE
comm_lib
raptor_lib
)