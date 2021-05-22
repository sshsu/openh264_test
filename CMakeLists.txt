cmake_minimum_required(VERSION 3.0.0)
project(openh264_encode VERSION 0.1.0)

link_directories("/usr/local/lib")

add_executable(openh264_encode main.cpp
                bufferReader.cpp)
target_link_libraries(openh264_encode -lrt -lopenh264)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

