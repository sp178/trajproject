# 生成选择 -G "MSYS Makefiles" 或者 "Makefiles"
cmake_minimum_required(VERSION 3.6.0 FATAL_ERROR)
#project(test)


#生成选项 Debug Release RelWithDebInfo  MinSizeRel 
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin) #执行文件路径
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ./bin)  #//动态库路径
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ./lib)  #静态库路径

#编译后文件后添加相应辨识标志
# if(WIN32 AND NOT MINGW)
# if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
#   set(CMAKE_DEBUG_POSTFIX "_debug")
# endif()
# if(NOT DEFINED CMAKE_RELEASE_POSTFIX)
#   set(CMAKE_RELEASE_POSTFIX "_release")
# endif()
# if(NOT DEFINED CMAKE_RELWITHDEBINFO_POSTFIX)
#   set(CMAKE_RELWITHDEBINFO_POSTFIX "_release")
# endif()
# if(NOT DEFINED CMAKE_MINSIZEREL_POSTFIX)
#   set(CMAKE_MINSIZEREL_POSTFIX "_release")
# endif()
# endif()
#set(GSL_ROOT_DIR "D:/spsys/GSL") #设置查找目录
find_package(GSL COMPONENTS gsl REQUIRED)
find_package(Boost COMPONENTS filesystem regex REQUIRED) 
# if(GSL_FOUND)
# message(STATUS "GSL:")
# message(STATUS "库文件目录:${GSL_LIBRARIES}")
# message(STATUS "头文件目录:${GSL_INCLUDE_DIR}")
# endif()
set(CMAKE_INSTALL_PREFIX "./")

if(Boost_FOUND AND GSL_FOUND)
include_directories(${Boost_INCLUDE_DIRS} ${GSL_INCLUDE_DIRS}) 

add_executable(main
./plantform/main.cpp
./plantform/recorder.cpp 
./plantform/modelbase.cpp 
./plantform/odeengin.cpp) 
target_link_libraries(main ${GLS_LIBRARIES} ${Boost_LIBRARIES} ${GSL_LIBRARIES})
set_target_properties(main PROPERTIES 
OUTPUT_NAME "test"  #名称
SUFFIX ".exe"           #后缀名
PREFIX "test")          #前缀名
#安装方法1
install(TARGETS main
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib/static
)
#安装方法2
#格式如下
install(DIRECTORY plantform/ DESTINATION include
FILES_MATCHING PATTERN "*.h"
PATTERN "*.cpp")
# PUBLIC_HEADER DESTINATION include
# FILES_MATCHING PATTERN "*.h"
# )

endif()