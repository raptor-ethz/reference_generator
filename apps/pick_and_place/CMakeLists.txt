add_executable(pick_and_place src/main.cpp)

target_link_libraries(pick_and_place PRIVATE
comm_lib
raptor_lib
)