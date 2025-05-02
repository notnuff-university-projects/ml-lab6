cmake_minimum_required(VERSION 3.16)
project(music_clustering)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/notnuff/Qt/6.8.1/gcc_64/lib/cmake")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(music_clustering
    main.cpp
    art1_algorithm.cpp
    mainwindow.cpp
)

target_link_libraries(music_clustering PRIVATE Qt6::Widgets) 