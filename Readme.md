# "低性能"服务器编程（Quill Plan）

> 针对向我一样C++初学者，Quill Plan 由浅入深，慢慢剖析服务器编程。
>
> 项目地址 : https://github.com/PushedChris/-Quill-Plan-

### TTWebSserver

Linux下超轻量级Web服务器。

- 使用多进程 + 阻塞IO实现并发模型，每个进程单独使用一个mysql连接
- 支持GET、POST
- 通过访问服务器数据库实现web端用户**注册、登录**功能，可以请求服务器**图片和视频文件**
- 实现实时Printf 日志系统，记录服务器运行状态
- 经压力测试可以实现**上千的并发连接**数据交换

### 程序框架

多进程优点:解耦、模块化、方便调及重用。

![TTWebServer](https://github.com/PushedChris/-Quill-Plan-/blob/master/TTWebServer.png)

### 基础测试

- 服务器测试环境
  - Ubuntu版本18.04
  - MYSQL版本5.7.29
  - CPU i5 8500
  - vmware虚拟机 2核
  
- 浏览器测试环境
  - Windows、Linux均可
  - Chrome
  - FireFox
  
- 测试前确认已安装MySQL数据库

  ```mysql
  // 建立yourdb库
  create database ttWebServer;
  
  // 创建user表
  USE yourdb;
  CREATE TABLE user(
      username char(50) NULL,
      passwd char(50) NULL
  )ENGINE=InnoDB;
  
  // 添加数据
  INSERT INTO user(username, passwd) VALUES('name', 'passwd');
  ```

- 修改webserver.cpp中的数据库初始化信息

  ```c++
  //line 80
  conn->init("localhost", "database_usr_name", "database_password", "database_name", 3306,1);
  ```

##### 程序运行

```shell
#默认 9999端口
./server [-p port]
```

### 压力测试

> QPS = req/sec = 请求数/秒
>
> 单台服务器每天PV计算
> 公式1：每天总PV = QPS * 3600 * 6
> 公式2：每天总PV = QPS * 3600 * 8



关掉日志系统(就是printf),使用Webbench对服务器进行压力测试.mysql支持最大并发数为100（此处可用连接池优化）

- QPS:35596

![TTserver压力测试](https://github.com/PushedChris/-Quill-Plan-/blob/master/TTserver%E5%8E%8B%E5%8A%9B%E6%B5%8B%E8%AF%95.PNG?raw=true)

- 测试数据

  - 并发连接总数：1010

  - 访问服务器时间：5s

  - 所有访问均成功

### 浏览器演示

### ![ttweb](https://github.com/PushedChris/-Quill-Plan-/blob/master/ttweb.gif?raw=true)