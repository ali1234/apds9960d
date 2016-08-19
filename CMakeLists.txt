cmake_minimum_required(VERSION 3.0)

PROJECT(apds9960d)

SET(apds9960d_SOURCES
  main.cpp
  i2cdevice.cpp
  apds9960.cpp
  backlight.cpp
  uinput.cpp
)

ADD_EXECUTABLE(${PROJECT_NAME} ${apds9960d_SOURCES})

ADD_DEFINITIONS("-std=gnu++11")

FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
