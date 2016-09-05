cmake_minimum_required(VERSION 2.8)
project(ardyno)
set(CMAKE_BUILD_TYPE Release)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(SOURCE_FILES
#    src/Dynamixel.cpp
#    src/Dynamixel.h
#    src/DynamixelConsole.cpp
#    src/DynamixelConsole.h
#    src/DynamixelInterface.cpp
#    src/DynamixelInterface.h
#    src/DynamixelMotor.cpp
#    src/DynamixelMotor.h src/SerialInterface.cpp src/SerialInterface.hpp src/main.cpp)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

#Generate the shared library from the sources
add_library(ardyno SHARED ${SOURCES})

#Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
#install(TARGETS ardyno DESTINATION /usr/lib)

#add_executable(ardyno ${SOURCE_FILES})