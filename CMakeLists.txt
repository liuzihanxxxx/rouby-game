cmake_minimum_required(VERSION 3.27)
project(rouby-game)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(SFML_DIR "D:/SFML-2.6.1/lib/cmake/SFML")

add_executable(RobiGame src/game/main.cpp)

include_directories(include)

find_package(SFML COMPONENTS graphics audio REQUIRED)
target_link_libraries(RobiGame sfml-graphics sfml-audio sfml-system opengl32)

# 复制DLLs到可执行文件目录的自定义命令
add_custom_command(TARGET RobiGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/SFML-2.6.1/bin/sfml-graphics-2.dll"
        "D:/SFML-2.6.1/bin/sfml-graphics-d-2.dll"
        "D:/SFML-2.6.1/bin/sfml-system-2.dll"
        "D:/SFML-2.6.1/bin/sfml-system-d-2.dll"
        "D:/SFML-2.6.1/bin/sfml-window-2.dll"
        "D:/SFML-2.6.1/bin/sfml-window-d-2.dll"
        "D:/SFML-2.6.1/bin/sfml-audio-2.dll"
        "D:/SFML-2.6.1/bin/sfml-audio-d-2.dll"
        "D:/SFML-2.6.1/bin/sfml-network-2.dll"
        "D:/SFML-2.6.1/bin/sfml-network-d-2.dll"
        "D:/SFML-2.6.1/bin/openal32.dll"
        $<TARGET_FILE_DIR:RobiGame>)

add_library(Genetic SHARED include/Genetic.h include/Genetic.cpp)
