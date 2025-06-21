### Office 开发相关库与 COM 组件开发笔记整理  

#### 一、Office 相关库说明  

| 库名称               | 库文件名      | `libid` GUID                                | 命名空间                  | 主要功能描述                                                                 |
|----------------------|---------------|---------------------------------------------|---------------------------|------------------------------------------------------------------------------|
| Microsoft Word       | MSWORD.OLB    | `{00020905-0000-0000-C000-000000000046}`     | `Word`                    | 提供 Word 的对象模型接口，用于自动化操作和程序化控制 Word 应用程序。         |
| Microsoft PowerPoint | MSPPT.OLB     | `{91493440-5A91-11CF-8700-00AA0060263B}`     | `PPT`                     | 提供 PowerPoint 的对象模型接口，支持创建、编辑和管理演示文稿内容。           |
| Microsoft Excel      | EXCEL.EXE     | `{00020813-0000-0000-C000-000000000046}`     | `Excel`                   | 提供 Excel 的对象模型接口，用于处理电子表格数据、公式、图表等元素。          |
| Microsoft Add-in Designer | MSADDNDR.DLL | `{AC0714F2-3D04-11D1-AE7D-00A0C90F26F4}` | `AddInDesignerObjects`   | 支持 Office 插件开发，可用于创建 COM 加载项、扩展 Office 功能等高级开发任务。|

---


#### 二、获取库 LIBID 的方法  
1. 使用 **Everything** 搜索库文件的存放路径。  
2. 下载并打开 **OleView 工具**，通过 `View TypeLib` 打开目标库。  
3. 在生成的 IDL 文件中查看 `LIBID` 和版本号。  


#### 三、开发中常见问题与解决方案  
##### 1. 导入 `msword.olb` 时的重复定义问题  
- **问题**：链接器提示 `msword.tlh` 和 `VBE6EXT.tlh` 中重复定义 `IID_AddIn`。  
- **解决**：导入时删除 `named_guids` 参数（通过排除法验证有效）。  

##### 2. 字符串类型转换  
- 将 `CString` 替换为 `CATLString`。  

##### 3. UI 界面开发  
- **Ribbon 界面**：  
  - 继承接口 `IRibbonExtensibility`，创建 `ribbon.xml` 文件，并实现 `GetCustomUI` 方法。  
- **MFC 资源文件**：  
  - 使用 VS 资源文件设计 UI，通过 `Atl 窗口类` 关联资源 ID（如 `IDC_STATIC`），通过 `GetDlgItem` 操作控件。  


#### 四、调试技巧  
1. **避免附加调试**：附加调试无法调试程序启动前的代码。  
2. **启动调试设置**：将 Office 程序（如 Word）设为调试命令，直接启动调试。  


#### 五、状态栏写入问题  
- **现状**：调用逻辑返回 `S_OK`，但状态栏无显示，可能因内容被刷新覆盖。  


#### 六、ATL 对话框类开发要点  
##### 1. 资源文件与对话框绑定  
- 在资源文件中创建资源并为资源和控件分配 ID。  
- 在 ATL 对话框类中通过 `enum { IDD = 资源ID }` 关联主窗口。
- 然后使用getDlgofname()获取控件对象，使用CWindow接收

##### 2. 消息映射（类似 Qt 信号槽）  
- **常用映射**：  
  - `WM_INITDIALOG` → `OnInitDialog`（初始化）  
  - `WM_CLOSE` → `OnClose`（关闭事件）  
  - `WM_DESTROY` → `OnDestroy`（窗口销毁）  
  - `WM_NCDESTROY` → `OnFinalMessage`（最终销毁，建议在此处 `delete this`）  
  - `OK/Cancel` → `OnOk/OnCancel`  

##### 3. 非模态对话框创建与销毁  
- **创建**：调用 `Create` 后 `ShowWindow`。  
- **销毁**：调用 `DestroyWindow`（异步操作，触发 `WM_DESTROY` 和 `WM_NCDESTROY`）。  
- **注意事项**：  
  - 避免手动 `delete this`，应在 `OnFinalMessage` 中处理。  
  - 销毁前需调用 `DestroyWindow`，并通过 `IsWindow()` 判断窗口有效性。  
  - 控件使用 `GetDlgItem` 获取后需 `Detach`，避免资源泄漏。  

##### 4. COM 组件跨线程限制  
- UI 线程中不能直接调用 COM 接口（COM 接口仅允许在创建线程内调用）。  
- STA：单线程   MTA：多线程  意：office中应用默认为STA。。
- 自由线程对象：可以随意的跨线程调用。


#### 七、事件类开发  
1. **继承 `IDispEventImpl`**：  
   - 参数说明：`IDispEventImpl<事件ID, 类, 接口ID, 库ID, 版本>`。  
2. **事件映射**：  
   - 通过 `BEGIN_SINK_MAP` 绑定事件与回调函数，格式为：  
     `SINK_ENTRY_EX(接口ID, 事件DISPID, 回调函数)`。  
3. **DISPID 获取**：  
   - 使用 `OleView.exe` 打开库，此时会产生该库的idl文件，然后到`interface` 中查找事件类对应的 `DISPID`。  


#### 八、错误信息支持（`ISupportErrorInfo`）  
1. 实现 `InterfaceSupportsErrorInfo` 接口，指定支持的接口 `IID`。  
2. 通过 `ICreateErrorInfo` 创建错误信息，调用 `GetErrorInfo()` 获取错误详情。  


#### 九、COM 开发语法规范  
- **类名前缀**：`_` 表示接口类，无前缀表示 COM 对象实现类。  
- **指针类型**：使用 `CComPtr`（类似 `shared_ptr`），如 `CComPtr<_Application>`，然后`_ApplicationPtr`等价于`CComPtr<_Application>`。
- **接口与库 ID**：通过 `__uuidof()` 获取，如 `__word` 对应 Word 库。  


#### 十、Office 插件注册方法  
1. **常规方法**：通过 Office 开发者工具的 COM 加载项添加（易失败）。  
2. **手动注册表方法**：  
   - 定位到 `HKEY_CURRENT_USER\Software\Microsoft\Office\应用程序\Addins`（如 Excel）。  
   - 新建项，命名为 COM 类的 `ProgID`（在 RGS 文件中查找）。  
   - 可选添加 `LoadBehavior`（建议设为 3，启动时加载）、`FriendlyName`、`Description`。
3. **自动注册方法**： 
   - 在插件的rgs文件中实现。



#### 十一、Ribbon UI 详细开发步骤  
1. 类继承 `office::IRibbonExtensibility`，添加 COM 映射。  
2. 重写 `GetCustomUI` 方法，加载 `ribbon.xml` 文件（Unicode 编码）。  
3. 在 IDL 接口中声明回调函数，并在 COM 类中实现，需与 XML 中 `onAction` 名称一致。  


#### 十二、调试与异常处理  
1. **调试痛点**：COM 组件加载失败时无报错信息。  
2. **解决方案**：将 Office 程序设为启动命令，在 `DllMain` 处打断点逐步调试。  
3. **异常处理工具**：  
   - `ATLTRACE("日志信息")`：Debug 模式打印日志。  
   - `ATLASSERT(条件)`：断言检查。  
   - `_com_error`：捕获 COM 异常（配合 `try-catch` 使用）。