cmake_minimum_required(VERSION 3.4.1)

project(calculator)

# 设置C++标准及标志
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

# 查找Qt5模块
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# 添加可执行文件
add_executable(calculator {CMAKE_CURRENT_SOURCE_DIR}/calculator/calculator/main.cpp {CMAKE_CURRENT_SOURCE_DIR}/calculator/calculator/mycalculatorEngine.cpp {CMAKE_CURRENT_SOURCE_DIR}/calculator/calculator/mycalculatorInterface.cpp)


# 包含Qt生成的文件目录（如moc, ui等）
set(CMAKE_AUTOMOC ON)    # 自动运行moc
set(CMAKE_AUTOUIC ON)    # 自动处理.ui文件
set(CMAKE_AUTORCC ON)    # 自动处理.qrc文件

# 将Qt Widgets模块链接到你的可执行文件
target_link_libraries(calculator Qt5::Widgets)


# 注意：上面的target_include_directories指令中的路径应该是指向目录而非单独的头文件。
# 如果mycalculatorEngine.h和mycalculatorInterface.h位于相同的目录下，你可以这样做：
target_include_directories(calculator PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/calculator/calculator
)