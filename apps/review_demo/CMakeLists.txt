add_executable(review_demo src/main.cpp src/graph.cpp)
target_include_directories(review_demo PUBLIC include)

target_link_libraries(review_demo PRIVATE
comm_lib
raptor_lib
)