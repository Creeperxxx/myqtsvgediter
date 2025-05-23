# 最小支持的CMake版本
cmake_minimum_required(VERSION 3.5)

# 项目名称和版本
project(kdemo VERSION 1.0)

# 指定使用的C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找Qt5组件
set(Qt5_DIR "D:/Application/Vcpkg/vcpkg/installed/x64-windows/share/cmake/Qt5")
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

# 添加可执行文件
add_executable(kdialog 
    main.cpp
    kdialog.cpp
    shadoweffecthelper.cpp
)

# 处理资源文件
# qt5_add_resources(KDIALOG_RESOURCES kdialog.qrc)

# 将资源添加到目标
# target_sources(kdialog PRIVATE ${KDIALOG_RESOURCES})

# 链接Qt5Widgets库
target_link_libraries(kdialog Qt5::Core Qt5::Gui Qt5::Widgets klogin kpurchase)

# 设置包含目录
target_include_directories(kdialog PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 如果有UI文件需要处理，可以使用以下命令
# qt5_wrap_ui(KDIALOG_UIS kdialog.ui)
# target_sources(kdialog PRIVATE ${KDIALOG_UIS})