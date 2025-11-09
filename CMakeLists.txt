cmake_minimum_required(VERSION 3.10)
project(Game CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Game
Game.cpp
)

add_custom_command(TARGET Game POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy
${CMAKE_CURRENT_SOURCE_DIR}/dic.txt
$<TARGET_FILE_DIR:Game>
)