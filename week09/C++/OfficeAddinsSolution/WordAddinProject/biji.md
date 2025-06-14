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
1. 










提示词：
1. ui设计方案：ATL 提供的能力结合 Office 的对象模型