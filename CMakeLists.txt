cmake_minimum_required(VERSION 3.10)

# 项目名称和使用的语言
project(mycodetest)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录
include_directories(include)

# 定义源文件变量
set(SOURCE_FILES
    src/main.cpp
    include/sample_head.cpp
    # 注意：不需要再次包含 sample_head.h
)

# 定义头文件变量
set(HEADER_FILES
    include/sample_head.h
)
# 添加可执行文件
add_executable(main ${SOURCE_FILES})