# 生成选择 -G "MSYS Makefiles" 或 "Makefiles"
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(utilyty)
INCLUDE(D:/msys64/mingw64/share/cmake-3.9/Modules/FindCUDA.cmake) 
#生成选项 Debug Release RelWithDebInfo  MinSizeRel 
SET(CMAKE_BUILD_TYPE Release)

set(CMAKE_INSTALL_PREFIX "./")
LIST(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_35,code=sm_35)
#set(CUDA_NVCC_FLAGS "-gencode arch=compute35,code=sm35")
CUDA_ADD_LIBRARY(utilyty
spmath.cu)
add_compile_options(utilyty -std=c++11) 

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
 IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
#    target_link_libraries(utilyty -ldl ) 
 ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")  

 endif()

set_target_properties(utilyty PROPERTIES 
OUTPUT_NAME "math"  #名称
SUFFIX ".spdll"           #后缀
PREFIX "")          #前缀
#安装方法1
install(TARGETS utilyty
RUNTIME DESTINATION ../bin
LIBRARY DESTINATION ../lib
ARCHIVE DESTINATION ../lib/static
)
#安装方法2
#格式如下
install(DIRECTORY plantform/ DESTINATION include
FILES_MATCHING PATTERN "*.h"
PATTERN "*.cpp"
PATTERN "*.cu"
PATTERN "*.cuh")
# PUBLIC_HEADER DESTINATION include
# FILES_MATCHING PATTERN "*.h"
# )

#endif()