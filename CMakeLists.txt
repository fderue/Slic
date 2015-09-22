cmake_minimum_required(VERSION 2.8)
project(Slic)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
#set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build"})



find_package(OpenCV REQUIRED)

include_directories(/usr/local/include; /usr/local/cuda/include)
link_directories(/usr/local/lib; /usr/local/cuda/lib)

add_executable(Slic main.cpp Slic.h Slic.cpp)

target_link_libraries(Slic ${OpenCV_LIBS})