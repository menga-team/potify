cmake_minimum_required(VERSION 3.21)
project(potify)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        REQUIRED)

add_executable(potify main.cpp controls.h controls.cpp playlist.h playlist.cpp player.h player.cpp)
target_link_libraries(potify
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
        )

