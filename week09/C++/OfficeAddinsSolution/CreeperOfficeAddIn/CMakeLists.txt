# CMake 最低版本要求
cmake_minimum_required(VERSION 3.20)

# 项目名称与语言设置
project(CreeperOfficeAddIn LANGUAGES CXX)

# 设置构建类型为 Release 或 Debug（可选）
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# 设置 MFC 支持（静态或共享）
set(CMAKE_MFC_FLAG 2) # 2 表示使用共享 MFC 库

# 启用 ATL 支持
add_definitions(-D_ATL_MIN_CRT -D_ATL_DLL_IMPL)


include_directories(${PROJECT_SOURCE_DIR})

# 收集所有源文件、头文件、资源文件
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.h" "*.tlh" "*.tli")
file(GLOB_RECURSE RESOURCE_FILES "*.rc" "*.rgs")

# 添加资源编译支持（Windows 资源文件）
enable_language(RC)

# 定义 DLL 目标
add_library(${PROJECT_NAME} SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${RESOURCE_FILES}
)

# 指定 RC 文件（资源编译器输入）
set_property(SOURCE CreeperOfficeAddIn.rc PROPERTY LANGUAGE RC)

# 链接必要的 Windows 库
target_link_libraries(${PROJECT_NAME} PRIVATE
    comctl32
    ole32
    uuid
    atlthunk
)

# 添加预处理器定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    _WINDOWS
    _USRDLL
    WIN32
    _UNICODE
    UNICODE
)

# 设置 ATL 支持标志
target_compile_definitions(${PROJECT_NAME} PRIVATE
    _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
    _AFX_ALL_WARNINGS
)

# 如果需要，可以添加额外依赖项
# target_include_directories(${PROJECT_NAME} PRIVATE "路径到其他COM库")

# 输出目录配置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 可选：指定输出 DLL 名称格式
set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    SUFFIX ".dll"
    OUTPUT_NAME "${PROJECT_NAME}"
)