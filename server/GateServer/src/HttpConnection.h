#pragma once
#include "const.h"
#include <boost/asio/steady_timer.hpp>
#include <boost/beast/http/message_fwd.hpp>
#include <unordered_map>
/*每当客户端连接到服务器时，服务器会为该连接创处理请建一个 HttpConnection 实例。
该实例负责与客户端的通信，包括接收请求、求和发送响应。
*/
class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
  friend class LogicSystem;
  HttpConnection(tcp::socket socket);
  void Start();

private:
  void CheckDeadline(); // 检查超时
  void WriteResponse(); // 写响应
  void HandleReq();     // 处理请求
  tcp::socket _socket;
  beast::flat_buffer _buffer{1892};           // 缓冲区大小8K
  http::request<http::dynamic_body> _request; // 动态请求对象
  http::response<http::dynamic_body> _response;
  net::steady_timer deadline_{_socket.get_executor(),
                              std::chrono::seconds(60)}; // 超时定时器

  std::string _get_url;
  std::unordered_map<std::string, std::string> _get_params; // 存储get请求的参数
  void PreParseGetParam(); // 预处理get请求参数
  unsigned char FromHex(unsigned char x);
  std::string UrlEncode(const std::string &str);
  std::string UrlDecode(const std::string &str);
  unsigned char ToHex(unsigned char x);
};
