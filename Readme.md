# "低性能"服务器编程（Quill Plan）

> 针对向我一样C++初学者，Quill Plan 由浅入深，慢慢剖析服务器编程。
>
> 项目地址 : https://github.com/PushedChris/-Quill-Plan-

Linux下超轻量级Web服务器。

- 使用多进程 + 阻塞IO实现并发模型
- 支持GET
- 通过访问服务器数据库实现web端用户**注册、登录**功能，可以请求服务器**图片和视频文件**
- 实现实时Printf 日志系统，记录服务器运行状态
- 经压力测试

### 基础测试

- 服务器测试环境
  - Ubuntu版本18.04
  - MYSQL版本5.7.29
- 浏览器测试环境
  - Windows、Linux均可
  - Chrome
  - FireFox