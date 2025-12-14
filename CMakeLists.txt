cmake_minimum_required(VERSION 3.10.0)
project(a_start_demo VERSION 0.1.0 LANGUAGES C CXX)

add_executable(win32_platform WIN32 src/win32_platform.cpp)

target_compile_options(win32_platform
    PRIVATE
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

target_link_options(win32_platform
    PRIVATE
        /SUBSYSTEM:WINDOWS,5.02 # To select the minimum compatible system
        /MAP
)

target_link_libraries(win32_platform
    PRIVATE
        winmm.lib
)

target_include_directories(win32_platform
    PRIVATE
        include/
)