# Source Code Publisher(源码发布器) (C++) 

- 支持搜索符合特定pattern和regex的源文件并将其转化为html文件. 

- 利用parser分析scopes和tokens为转化的HTML添加依赖并隐藏显示各功能模块.

- 设计GUI (C#) 打开，搜索，修改和浏览源代码&提供Shim包来建立GUI(C#)和源 码发布器(C++)之间的通信.

- 实施communication channel & 设计messages用于远程操作&develop a server to accept request message & 通过C++/CLI translater.连接Client GUI与Publish Server

- You could directly use ***run.bat*** or run ***CodePublisher.sln*** to run application