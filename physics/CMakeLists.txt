# physics/CMakeLists.txt
add_library(physics STATIC src/world.cpp)
target_include_directories(physics PUBLIC include)
target_link_libraries(physics PUBLIC core)
