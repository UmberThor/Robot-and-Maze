cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(maze_robot) 

include_directories(include) 

add_library(Maze
	include/Maze.h
	src/Maze.cpp
)

add_library(Robot
	include/Robot.h
	src/Robot.cpp
)

add_executable(SolveMaze src/Main.cpp)

target_link_libraries(SolveMaze Maze Robot)

install(TARGETS SolveMaze DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)