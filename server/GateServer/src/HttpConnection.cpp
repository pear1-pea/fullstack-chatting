#include "HttpConnection.h"
#include "LogicSystem.h"
#include <boost/beast/http/field.hpp>
HttpConnection::HttpConnection(tcp::socket socket)
    : _socket(std::move(socket)) {}

void HttpConnection::Start() {
  auto self = shared_from_this();
  // boost::asio::async_read(s, boost::asio::buffer(data, size), handler);
  // 异步读取客户端发送的http请求,取到的数据存储在 _buffer 中，并解析为 _request
  // 对象
  http::async_read(_socket, _buffer, _request,
                   [self](beast::error_code ec, std::size_t bytes_transferred) {
                     try {
                       if (ec) {
                         std::cout << "http read err id " << ec.what()
                                   << std::endl;
                       }

                       boost::ignore_unused(bytes_transferred);
                       self->HandleReq();     // 处理请求
                       self->CheckDeadline(); // 检查超时
                     } catch (std::exception &exp) {
                       std::cout << "exception is " << exp.what() << std::endl;
                     }
                   });
}

void HttpConnection::HandleReq() {
  // 设置版本
  _response.version(_request.version()); // 设置一致的响应版本
  _response.keep_alive(false);           // http短链接

  if (_request.method() == http::verb::get) {
    PreParseGetParam();
    bool success =
        LogicSystem::GetInstance()->HandleGet(_get_url, shared_from_this());
    // 处理get请求
    if (!success) {
      _response.result(http::status::not_found);
      _response.set(http::field::content_type, "text/plain"); // 设置回包类型
      beast::ostream(_response.body()) << "url not founs\r\n";
      WriteResponse();
      return;
    }
    _response.result(http::status::ok);
    _response.set(http::field::server, "GateServer"); // 告诉是哪个服务器回包的
    WriteResponse();
    return;
  }

  if (_request.method() == http::verb::post) {
    // 处理post请求
    bool success = LogicSystem::GetInstance()->HandlePost(_request.target(),
                                                          shared_from_this());
    if (!success) {
      _response.result(http::status::not_found);
      _response.set(http::field::content_type, "text/plain");
      beast::ostream(_response.body()) << "url not found\r\n";
      WriteResponse();
      return;
    }
    _response.result(http::status::ok);
    _response.set(http::field::server, "GateServer");
    WriteResponse();
    return;
  }
}

void HttpConnection::WriteResponse() {
  auto self = shared_from_this();
  _response.content_length(_response.body().size());
  http::async_write(
      _socket, _response,
      [self](beast::error_code ec, std::size_t bytes_transferred) {
        self->_socket.shutdown(tcp::socket::shutdown_send, ec);
        self->deadline_.cancel();
      });
}

void HttpConnection::CheckDeadline() {
  auto self = shared_from_this();
  // 如果超时，调用回调函数
  deadline_.async_wait([self](beast::error_code ec) {
    if (!ec) {
      self->_socket.close(ec); // 服务器一般不要主动去关客户端
    }
  });
}

void HttpConnection::PreParseGetParam() {
  // 提取 URI
  auto uri = _request.target();
  // 查找查询字符串的开始位置（即 '?' 的位置）
  auto query_pos = uri.find('?');
  if (query_pos == std::string::npos) {
    _get_url = uri;
    return;
  }

  _get_url = uri.substr(0, query_pos);
  std::string query_string = uri.substr(query_pos + 1);
  std::string key;
  std::string value;
  size_t pos = 0;
  while ((pos = query_string.find('&')) != std::string::npos) {
    auto pair = query_string.substr(0, pos);
    size_t eq_pos = pair.find('=');
    if (eq_pos != std::string::npos) {
      key = UrlDecode(
          pair.substr(0, eq_pos)); // 假设有 url_decode 函数来处理URL解码
      value = UrlDecode(pair.substr(eq_pos + 1));
      _get_params[key] = value;
    }
    query_string.erase(0, pos + 1);
  }
  // 处理最后一个参数对（如果没有 & 分隔符）
  if (!query_string.empty()) {
    size_t eq_pos = query_string.find('=');
    if (eq_pos != std::string::npos) {
      key = UrlDecode(query_string.substr(0, eq_pos));
      value = UrlDecode(query_string.substr(eq_pos + 1));
      _get_params[key] = value;
    }
  }
}

unsigned char HttpConnection::FromHex(unsigned char x) {
  unsigned char y;
  if (x >= 'A' && x <= 'Z') {
    y = x - 'A' + 10;
  } else if (x >= '0' && x <= '9') {
    y = x - 'a' + 10;
  } else {
    assert(0);
  }
  return y;
}

std::string HttpConnection::UrlEncode(const std::string &str) {
  std::string strTemp = "";
  size_t length = str.length();
  for (size_t i = 0; i < length; i++) {
    if (isalnum((unsigned char)str[i]) || (str[i] == '-') || (str[i] == '-') ||
        (str[i] == '.') || (str[i] == '~'))
      strTemp += str[i];
    else if (str[i] == ' ') {
      strTemp += "+";
    } else {
      strTemp += '%';
      strTemp += ToHex((unsigned char)str[i] >> 4);
      strTemp += ToHex((unsigned char)str[i] & 0x0F);
    }
  }
  return strTemp;
}

std::string HttpConnection::UrlDecode(const std::string &str) {
  std::string strTemp = "";
  size_t length = str.length();
  for (size_t i = 0; i < length; i++) {
    if (str[i] == '+')
      strTemp += " ";
    else if (str[i] == '%') {
      assert(i + 2 < length);
      unsigned char high = FromHex((unsigned char)str[++i]);
      unsigned char low = FromHex((unsigned char)str[++i]);
      strTemp += high * 16 + low;
    } else
      strTemp += str[i];
  }
  return strTemp;
}

unsigned char HttpConnection::ToHex(unsigned char x) {
  return x > 9 ? x + 55 : x + 48;
}
