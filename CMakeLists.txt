cmake_minimum_required(VERSION 3.6)
project(ardyno)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/Dynamixel.cpp
    src/Dynamixel.h
    src/DynamixelConsole.cpp
    src/DynamixelConsole.h
    src/DynamixelInterface.cpp
    src/DynamixelInterface.h
    src/DynamixelMotor.cpp
    src/DynamixelMotor.h src/SerialInterface.cpp src/SerialInterface.hpp)

add_executable(ardyno ${SOURCE_FILES})