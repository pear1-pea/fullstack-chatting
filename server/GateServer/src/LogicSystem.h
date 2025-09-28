#pragma once
#include "HttpConnection.h"
#include "const.h"

class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)>
    HttpHandler; // 处理器
class LogicSystem : public Singleton<LogicSystem> {
  friend class Singleton<LogicSystem>;

public:
  ~LogicSystem();
  bool HandleGet(std::string, std::shared_ptr<HttpConnection>);
  bool HandlePost(std::string, std::shared_ptr<HttpConnection>);
  void RegGet(std::string url, HttpHandler handler);
  void RegPost(std::string url, HttpHandler handler);

private:
  LogicSystem();
  // 映射表，_get_handlers存储GET请求
  std::map<std::string, HttpHandler> _post_handlers;
  std::map<std::string, HttpHandler> _get_handlers;
};
