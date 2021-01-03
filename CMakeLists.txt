cmake_minimum_required(VERSION 3.5)

project(Demo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()


set(OpenCV_DIR D:/opencv/OpenCV)
find_package(OpenCV REQUIRED)

if(NOT OpenCV_FOUND)
    message(FATAL_ERRO "failed to find OpenCV")
endif(NOT OpenCV_FOUND)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Network REQUIRED)

set(TS_FILES Demo_zh_CN.ts)
message("${PROJECT_SOURCE_DIR}")
if(ANDROID)
  add_library(Demo SHARED
    Detect/detect.cpp
    Detect/detect.h
    beginbutton.cpp
    beginbutton.h
    main.cpp
    ui.cpp
    ui.h
    ${PROJECT_SOURCE_DIR}/${TS_FILES}
  )
else()
  add_executable(Demo
    Detect/detect.h
    Detect/detect.cpp
    beginbutton.cpp
    beginbutton.h
    main.cpp
    ui.cpp
    ui.h
    #${PROJECT_SOURCE_DIR}/${TS_FILES}
  )
endif()


target_include_directories(Demo
    PUBLIC
    ${OpenCV_INCLUDES}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Detect
)
target_link_libraries(Demo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Demo PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Demo PUBLIC "${OpenCV_LIBS}")


#qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
