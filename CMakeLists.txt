cmake_minimum_required (VERSION 3.6)
project (producer_consumer_solution CXX)

set(CMAKE_CXX_STANDARD 14)

# on UNIX systems find pthreads.h library for compilation
if(UNIX AND NOT CYGWIN)
    set(CMAKE_CXX_FLAGS "-O3 -Wall -lncurses -lmenu -ltinfo -static-libgcc -static-libstdc++")
    find_package(Threads REQUIRED)
else(UNIX AND NOT CYGWIN)
    set(CMAKE_CXX_FLAGS "-O3 -Wall -lncurses -lmenu -static-libgcc -static-libstdc++")
endif(UNIX AND NOT CYGWIN)


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
        ProducerConsumer/
   )


foreach(SOURCE_DIRECTORY ${SOURCE_DIRECTORIES})

    set(${SOURCE_DIRECTORY}_REGEX ${SOURCE_DIRECTORY}*.cpp)
    file(GLOB_RECURSE ${SOURCE_DIRECTORY}_FILES ${${SOURCE_DIRECTORY}_REGEX})
    list(APPEND SOURCE_FILES ${${SOURCE_DIRECTORY}_FILES})

endforeach()


list(APPEND SOURCE_FILES main.cpp)
add_executable(bin ${SOURCE_FILES})

# compile different set of libraries on different platforms
if(UNIX AND NOT CYGWIN)
    target_link_libraries(bin Threads::Threads curses menu)
else(UNIX AND NOT CYGWIN)
    target_link_libraries(bin curses menu)
endif(UNIX AND NOT CYGWIN)

target_include_directories(bin PRIVATE ${CMAKE_SOURCE_DIR})