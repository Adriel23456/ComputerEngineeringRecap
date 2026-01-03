cmake_minimum_required(VERSION 3.16)
project(tinyfiledialogs LANGUAGES CXX)

add_library(tinyfiledialogs STATIC
    tinyfiledialogs.cpp
)

target_include_directories(tinyfiledialogs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# TinyFD on Windows needs these system libs
if(WIN32)
    target_link_libraries(tinyfiledialogs PUBLIC
        comdlg32
        ole32
        user32
        shell32
    )
endif()