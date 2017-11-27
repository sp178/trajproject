project(test)
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
# set(sourcefiles cpufunc.cpp \
# cudafunc.cu \
# spaero.cpp \
# spdynamic.cpp \
# spguidance.cpp \
# spmass.cpp \
# spmodel.cpp \
# sptraj.cpp \
# ../utility/spmath.cu)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  execute_process(command nvcc cpufunc.cpp 
  cudafunc.cu 
  spaero.cpp 
  spdynamic.cpp 
  spguidance.cpp 
  spmass.cpp 
  spmodel.cpp 
  sptraj.cpp 
  ../utility/spmath.cu
  -g -DDEBUG -I/usr/include/eigen3 -use_fast_math --cudart static -shared --relocatable-device-code=true -Xcompiler -fPIC -std=c++11 -gencode arch=compute_52,code=compute_52 -gencode arch=compute_52,code=sm_52  -link -o  "models.spdll" -lboost_locale
)  
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    execute_process(COMMAND nvcc   cpufunc.cpp 
    cudafunc.cu 
    spaero.cpp 
    spdynamic.cpp 
    spguidance.cpp 
    spmass.cpp 
    spmodel.cpp 
    sptraj.cpp 
    ../utility/spmath.cu
    -O3 -DNDEBUG -I/usr/include/eigen3 -use_fast_math --cudart static -shared --relocatable-device-code=true -Xcompiler -fPIC -std=c++11 -gencode arch=compute_52,code=compute_52 -gencode arch=compute_52,code=sm_52  -link -o  "models.spdll" -lboost_locale
    )  
else()
    execute_process(COMMAND nvcc  
    cpufunc.cpp 
    cudafunc.cu 
    spaero.cpp 
    spdynamic.cpp 
    spguidance.cpp 
    spmass.cpp 
    spmodel.cpp 
    sptraj.cpp 
    ../utility/spmath.cu
    -O3 -DNDEBUG -I/usr/include/eigen3 -use_fast_math --cudart static -shared --relocatable-device-code=true -Xcompiler -fPIC -std=c++11 -gencode arch=compute_52,code=compute_52 -gencode arch=compute_52,code=sm_52  -link -o  "models.spdll" -lboost_locale
    )  
endif()





# #生成选项 Debug Release RelWithDebInfo  MinSizeRel 
# SET(CMAKE_BUILD_TYPE Debug)

# set(CMAKE_CXX_COMPILER "/usr/bin/g++")
# set(BOOST_ROOT_DIR "/usr/local")
# find_package(GSL COMPONENTS gsl REQUIRED)
# find_package(Boost COMPONENTS filesystem regex system REQUIRED) 
# find_package(CUDA REQUIRED)

# set(CMAKE_INSTALL_PREFIX "./")

# if(Boost_FOUND AND GSL_FOUND AND CUDA_FOUND)
# include_directories(${Boost_INCLUDE_DIRS} ${GSL_INCLUDE_DIRS} ${CUDA_INCLUDE_DIR}) 
# endif()

# set(CUDA_COM)

# set(CUDA_NVCC_FLAGS
# "-O3 
# -I/usr/include/eigen3 
# -use_fast_math 
# --cudart static 
# -shared 
# --relocatable-device-code=true 
# -Xcompiler -fPIC 
# -gencode arch=compute_52,code=compute_52 
# -gencode arch=compute_52,code=sm_52  
# -lboost_locale"
# ) 
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11" )


# include_directories("/usr/include/eigen3")


# cuda_add_library(dll 
# cpufunc.cpp 
# cudafunc.cu
# spaero.cpp
# spdynamic.cpp
# spguidance.cpp
# spmass.cpp
# spmodel.cpp
# sptraj.cpp
# ../utility/spmath.cu
# SHARED
# )

# target_link_libraries(dll ${GLS_LIBRARIES} ${Boost_LIBRARIES} ${GSL_LIBRARIES})

# set_target_properties(dll PROPERTIES 
# OUTPUT_NAME "models"  #名称
# SUFFIX ".spdll"           #后缀
# PREFIX "")          #前缀


# #安装方法1
# install(TARGETS dll
# RUNTIME DESTINATION bin
# LIBRARY DESTINATION lib
# ARCHIVE DESTINATION lib/static
# )
# #安装方法2
# #格式如下
# install(DIRECTORY plantform/ DESTINATION include
# FILES_MATCHING PATTERN "*.h"
# PATTERN "*.cpp")
# PUBLIC_HEADER DESTINATION include
# FILES_MATCHING PATTERN "*.h"
# )