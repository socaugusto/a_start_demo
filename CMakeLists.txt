cmake_minimum_required(VERSION 3.10.0)
project(a_start_demo VERSION 0.1.0 LANGUAGES C CXX)

add_executable(astar_demo WIN32
    src/win32_platform.cpp
    src/demo.cpp
)

target_compile_options(astar_demo
    PRIVATE
        /wd4201 # allow nameless struct/union

        /Oi
        /Od
        /GR-
        /Gm-
        /EHa
        /WX
        /W4
        /FC
        /Z7
        /nologo

        /MTd # C runtime library (Include library in executable)
)

target_link_options(astar_demo
    PRIVATE
        /SUBSYSTEM:WINDOWS,5.02 # To select the minimum compatible system
        /MAP
)

target_link_libraries(astar_demo
    PRIVATE
        winmm.lib
)

target_include_directories(astar_demo
    PRIVATE
        include/
)