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