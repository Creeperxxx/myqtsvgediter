cmake_minimum_required(VERSION 3.16)
project(Kdemo VERSION 1.0 LANGUAGES CXX)

# 设置 C++ 标准（建议 C++11 及以上）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "D:/Application/Vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")


# 添加子模块（按模块路径添加）
add_subdirectory(kdemo)
add_subdirectory(kpurchase)
add_subdirectory(klogin)