#ifndef CSERVER_H
#define CSERVER_H
#include "HttpConnection.h"
#include "const.h"
/*这是一个具体的实现类
*主要功能是创建一个acceptor对象，监听接收指定端口的连接请求，
端点定义了服务器监听的网络协议以及绑定的地址和端口
*/
class CServer : public std::enable_shared_from_this<CServer> {
public:
  CServer(boost::asio::io_context &ioc, unsigned short &port);
  void Start();

private:
  tcp::acceptor _acceptor; // 接收器
  net::io_context &_ioc;
  tcp::socket _socket;
};
#endif // CSERVER_H
