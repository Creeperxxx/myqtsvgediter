库：
	- mso.dll: Microsoft Office Shared Library, 多个应用程序共享的库
	- msaddndr.dll: Microsoft Office Add-in Library, 支持 Office 扩展和插件的开发
	- msppt.olb: 提供了对 PowerPoint 对象模型的访问
	- msword.olb : 提供了对 Word 对象模型的访问
	- msexcel.olb : 提供了对 Excel 对象模型的访问

目前不用包含office也就是mso.dll了，因为olb中会包含这个。

2. iwordaddin接口中有一个方法为	CountWords

3. import msword.olb时注意，链接器提示msword.tlh和VBE6EXT.tlh中会重复定义IID_AddIn。
	解决方法：只需要删掉import时的named_guids即可。查了好久最后排除法一点点试出来了。

4. cstring换catlstring

5. ui界面
    使用ribbon，继承接口：IRibbonExtensibility，创建ribbon.xml文件，并实现GetCustomUI方法
6. 解决不了的问题：
    在word中添加com加载项。
    解决方法：在word插件对应的注册表路径那里手动注册，记住：插件名称必须和com组件的progid一致，这样word才能找到对应的com组件。
1. 调试
	不要使用附加调试，那样必须启动word才能调试，导致启动完成前的代码无法调试。
    解决方法：使用启动调试，将word程序设置为调试命令。
1. ui设计：
	使用vs 的资源文件，也就是使用mfc来设计ui。
1. 向 Word 状态栏写入信息（集成式）
	最终：失败。逻辑上返回了S_ok,但是状态栏没有显示，原因可能是被刷新掉了？
1. 可以使用资源文件设计UI。每个控件都有自己的id，例如IDC_STATIC。
	资源的使用方法是，创建一个atl 窗口类，然后在enum中指定id为资源文件的id。还有就是使用getDlgItem获取id对应的控件
1. 只有在实现 COM 接口方法时才需要使用 STDMETHOD(func)(...) 这种写法。普通函数不需要这样写。
1. 关于atl的对话框类，非model启动，比较复杂。注意的点有以下：
	1. 配合资源文件创建对话框：
	    1. 首先创建资源文件，每个控件都对应一个id用来标记控件。
		1. 在atl对话框类中使用资源文件中设计的对话框，只需要enum {IDD = id},将id设置为主窗口的id即可
	1. BEGIN_MSG_MAP意思是，将消息映射到对应的处理函数。可以理解为qt中的信号和槽。一般要映射的有以下：		    1. WM_INITDIALOG对OnInitDialog，初始化对话框，create时会调用
		1. WM_CLOSE对OnCLose，点击叉会调用
		1. WM_DESTROY对OnDestroy。调用destroyWindow时会发出。
		1. WM_NCDESTROY对应OnFinalMessage。这是调用destroyWindow最后的一个消息，一般delete this。
		1. ok和cancel对应OnOk和OnCancel。
	1. 创建和销毁非模态对话框：
		1. 创建：create函数，然受showwindow。销毁：destroyWindow。用来销毁
		1. 销毁对话框时，需要调用destroyWindow，异步函数，依次发出WM_Destroy,WM_NCDestroy。
		    常见错误：
		    1. 销毁c++对象早于销毁窗口。意思是，delete this时，窗口还没销毁，因为destroywindow是异步的，
                所以不能手动delete，要在destroyWindow中delete，最好在OnFinalMessage。因此也用不了智能指针。
			1. m_hwnd有效或不为空。意思是没有调用destroyWindow，这个函数得手动调用或者绑定事件处理，例如在OnClose中调用。
			    要使用isWindow()函数判断窗口是否有效。
			1. 不要在析构中调用destroywindow，因为第一点那个，destroywindow要早于delete。
		1. destroywindow后可以create重新创建。
		1. 通过控件id使用getDlgItem获取控件，用完后要detach。
		1. 方法内不能使用com组件。常见为：一个按钮的回调函数内使用com组件，但是com组件的接口不能跨线程调用。
			com接口只允许在创建它的线程内调用，所以ui线程内是不能调用com中接口的。
1. 关于事件类：
    1. 继承IDispEventImpl，第一个参数为id，从1开始取，第三个为类型的id，第四个为库的id，最后为版本
	1. 通过BEGIN_SINK_MAP来绑定类型的事件与其回调函数。参数依次为id 类型id 事件dispid(就是idl中指明了的) 回调函数
	1. 使用DispEventAdvise来绑定某个对象到该事件类，DispEventUnAdvise解绑。如果继承了多个类型的IDispEventImpl，须指明类型。
	1. 怎么找dispid？使用oleview.exe找到对应的dll库，可以查看对应的idl文件，查看dispid。








提示词：
1. ui设计方案：ATL 提供的能力结合 Office 的对象模型