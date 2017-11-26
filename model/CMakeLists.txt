project(test)
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)



#生成选项 Debug Release RelWithDebInfo  MinSizeRel 
SET(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(BOOST_ROOT_DIR "/usr/local")
find_package(GSL COMPONENTS gsl REQUIRED)
find_package(Boost COMPONENTS filesystem regex system REQUIRED) 
find_package(CUDA REQUIRED)

set(CMAKE_INSTALL_PREFIX "./")

if(Boost_FOUND AND GSL_FOUND AND CUDA_FOUND)
include_directories(${Boost_INCLUDE_DIRS} ${GSL_INCLUDE_DIRS} ${CUDA_INCLUDE_DIR}) 
endif()

set(CUDA_COM)

# set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} 
# --relocatable-device-code=true
# -gencode arch=compute_52,code=compute_52 
# -gencode arch=compute_52,code=sm_52
# -cudart static 
# -Xcompiler -fPIC
# -use_fast_math 
# ) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11" )


include_directories("/usr/include/eigen3")


cuda_add_library(dll 
cpufunc.cpp 
cudafunc.cu
spaero.cpp
spdynamic.cpp
spguidance.cpp
spmass.cpp
spmodel.cpp
sptraj.cpp
../utility/spmath.cu
SHARED
OPTIONS
--relocatable-device-code=true
-gencode arch=compute_52,code=compute_52 
-cudart static 
-Xcompiler -std=c++11
)

target_link_libraries(dll ${GLS_LIBRARIES} ${Boost_LIBRARIES} ${GSL_LIBRARIES})

set_target_properties(dll PROPERTIES 
OUTPUT_NAME "models"  #名称
SUFFIX ".spdll"           #后缀
PREFIX "")          #前缀


#安装方法1
install(TARGETS dll
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