cmake_minimum_required (VERSION 3.6)
project (producer_consumer_solution CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3 -Wall -lncurses -lmenu -static-libgcc -static-libstdc++")


#######################################################
#                                                     #
# Add new directories with *.cpp files to this list   #
# All source code should be compiled in build process #
#                                                     #
# Author: Wolanski Grzegorz                           #
#                                                     #
#######################################################
set(SOURCE_DIRECTORIES
        Console/
        Miscellanous/
   )


foreach(SOURCE_DIRECTORY ${SOURCE_DIRECTORIES})

    set(${SOURCE_DIRECTORY}_REGEX ${SOURCE_DIRECTORY}*.cpp)
    file(GLOB_RECURSE ${SOURCE_DIRECTORY}_FILES ${${SOURCE_DIRECTORY}_REGEX})
    list(APPEND SOURCE_FILES ${${SOURCE_DIRECTORY}_FILES})

endforeach()


list(APPEND SOURCE_FILES main.cpp)
add_executable(bin ${SOURCE_FILES})
target_link_libraries(bin curses menu)
target_include_directories(bin PRIVATE ${CMAKE_SOURCE_DIR})