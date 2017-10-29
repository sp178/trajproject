# 生成选择 -G "MSYS Makefiles" 或者 "Makefiles"
cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
project(test)
find_package(Boost COMPONENTS filesystem regex REQUIRED) 
find_package(GSL)

if(Boost_FOUND&&GSL_FOUND)
include_directories(${Boost_INCLUDE_DIRS} ${GSL_INCLUDE_DIRS}) 
add_executable(main.hehe  ./plantform/main.cpp ./plantform/recorder.cpp ./plantform/modelbase.cpp ./plantform/odeengin.cpp) 
target_link_libraries(test ${Boost_LIBRARIES} ${GLS_LIBRARY})
endif()