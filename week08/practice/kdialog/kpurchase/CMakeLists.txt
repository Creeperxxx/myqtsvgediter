# 最小支持的 CMake 版本
cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(kpurchase VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR})

# 定义源文件列表
set(SOURCES
    kpurchase.cpp
    pch.cpp
)

# 定义头文件列表（如果需要）
set(HEADERS
    framework.h
    kpurchase.h
    pch.h
)

# 添加可执行文件或库
add_executable(kpurchase ${SOURCES} ${HEADERS})

# 如果有其他依赖库，可以在这里链接
target_link_libraries(kpurchase PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)

# 如果有预编译头文件，可以在这里设置
target_precompile_headers(kpurchase PRIVATE "pch.h")