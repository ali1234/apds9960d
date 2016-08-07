cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")

PROJECT(apds9960d)

SET(apds9960d_SOURCES
  main.cpp
  i2cdevice.cpp
  apds9960.cpp
  backlight.cpp
  uinput.cpp
)

ADD_EXECUTABLE(${PROJECT_NAME} ${apds9960d_SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
