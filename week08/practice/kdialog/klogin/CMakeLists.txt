# 最小支持的 CMake 版本
cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(klogin VERSION 1.0.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt5 并指定需要的组件
set(Qt5_DIR "D:/Application/Vcpkg/vcpkg/installed/x64-windows/share/cmake/Qt5")
find_package(Qt5  COMPONENTS Core Gui Widgets REQUIRED)
#  AxServer AxContainer
# 添加包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 添加源文件
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

# 创建共享库（DLL）
add_library(klogin SHARED ${SOURCES} ${HEADERS})

# 链接 Qt5 库
target_link_libraries(klogin PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)

# 设置预处理器定义
target_compile_definitions(klogin PRIVATE KLOGIN_LIBRARY)

# 如果有预编译头文件，可以这样设置
if(MSVC)
    target_precompile_headers(klogin PRIVATE "pch.h")
endif()

# 设置输出目录
set_target_properties(klogin PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# # 可选：安装目标
# install(TARGETS klogin DESTINATION bin)
# install(FILES ${HEADERS} DESTINATION include/klogin)