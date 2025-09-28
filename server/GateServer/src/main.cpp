#include "CServer.h"
#include "ConfigMgr.h"
#include <cstdlib>
#include <iostream>
/*在网络通信中，需要一个io_context,一个socket。我们创建一个context对象并设置线程池大小为1，即单线程运行。创建一个signal_set对象，用于捕获系统信号。
采用异步等待信号，传入是否出错并确定信号编号后，调用自定义回调函数，如果没出错，关闭服务器（优雅！）*/
/*graph TD
    A[客户端请求] --> B[CServer接受连接]
    B --> C[创建HttpConnection]
    C --> D[异步读取HTTP请求]
    D --> E{判断请求类型}
    E -->|GET| F[PreParseGetParam解析参数]
    E -->|POST| G[直接处理POST请求]
    F --> H[LogicSystem处理]
    G --> H
    H --> I[写回响应]
    I --> J[关闭连接]*/
int main() {
  ConfigMgr gCfgMgr;
  std::string gate_port_str = gCfgMgr["Gateserver"]["Port"];
  unsigned short gate_port = atoi(gate_port_str.c_str());
  try {
    unsigned short port = static_cast<unsigned short>(8080);
    net::io_context ioc{1};
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait(
        [&ioc](const boost::system::error_code &error, int signal_number) {
          if (error) {
            return;
          }
          ioc.stop();
        });
    std::make_shared<CServer>(ioc, port)
        ->Start(); // 创建CServer类的智能指针并调用start方法
    ioc.run();
  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
