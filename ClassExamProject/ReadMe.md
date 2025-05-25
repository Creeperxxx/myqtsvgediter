
# svgEditor 项目说明

## 项目简介
svgEditor 是一个基于 Qt 5.15.5 开发的图形编辑程序，使用 CMake 构建系统，并通过 vcpkg 管理第三方依赖。项目提供图形绘制、属性编辑、界面布局等功能，适用于svg编辑场景。


## 技术文档
【金山文档】 Qt SVG 编辑器项目开发技术文档
https://p.kdocs.cn/s/BQSCQTBAADAFA

## 构建与运行

### 步骤 1：安装依赖（通过 vcpkg）
```bash
vcpkg install qt5-base:x64-windows qt5-svg:x64-windows qt5-network:x64-windows
```

### 步骤 2：生成项目文件
```bash
mkdir build && cd build

cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="vcpkg根目录/scripts/buildsystems/vcpkg.cmake"
```

### 步骤 3：编译与运行
```bash
cmake --build . --config Release
./DiagramEditor.exe  
```


## 项目配置说明
### CMake 配置文件（`CMakeLists.txt`）
- **Qt 模块依赖**：通过 `find_package(Qt5 COMPONENTS ...)` 指定所需模块（如 `Widgets`、`Svg`）。  
- **自动处理工具**：  
  - `CMAKE_AUTOMOC`：自动生成 Qt 元对象代码（处理 `Q_OBJECT` 宏）。  
  - `CMAKE_AUTOUIC`：自动编译 UI 文件（`.ui`）。  
  - `CMAKE_AUTORCC`：自动编译资源文件（`.qrc`）。  
- **可执行文件属性**：  
  - `WIN32_EXECUTABLE ON`：Windows 下生成无控制台窗口的 GUI 程序。  
  - `MACOSX_BUNDLE ON`：macOS 下生成应用程序包（`.app`）。  


## 功能模块说明
| 模块名称               | 功能描述                                                                 |
|------------------------|--------------------------------------------------------------------------|
| `MainWindow`           | 主窗口逻辑，包含菜单、工具栏、工作区布局                                |
| `Diagram`              | 图形数据模型，处理图形节点、连接关系及布局逻辑                          |
| `Drawer`               | 图形渲染模块，基于 Qt 绘图框架实现可视化绘制                            |
| `PropertyWidget`       | 属性编辑组件，支持动态生成属性表单并与图形数据绑定                      |
| `Config`               | 程序配置管理，处理用户偏好设置、主题样式及资源加载                      |

