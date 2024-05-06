# cmake_minimum_required(2.8)
project(utils)

find_library(mysyalr "/home/sysu/GanProject/thrid-party/mysylar/lib/libmysylar.so")
# set(utils 
# )
# include_directories()
include_directories("/home/sysu/GanProject/thrid-party/mysylar/mysylar")
add_executable(util ganconfig.cpp)
target_link_libraries(util mysylar)

