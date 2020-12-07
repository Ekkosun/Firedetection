cmake_minimum_required(VERSION 3.5)

project(Test)

set(OpenCV_DIR D:/opencv/OpenCV)

find_package(OpenCV REQUIRED)

if(NOT OpenCV_FOUND)
    message(FATAL_ERRO "failed to find OpenCV")
endif(NOT OpenCV_FOUND)


add_executable(Test detect.cpp)


target_include_directories(Test
    PUBLIC
    ${OpenCV_INCLUDES}  
    ${PROJECT_SOURCE_DIR}
)
target_link_libraries(Test "${OpenCV_LIBS}")
