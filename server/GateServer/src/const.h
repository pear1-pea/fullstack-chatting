#pragma once
#include "Singleton.h"
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <functional>
#include <iostream>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <map>
#include <memory>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

enum ErrorCodes {
  Success = 0,
  Error_Json = 1001,
  RPCFailed = 1002,
};

class ConfigMgr;
extern ConfigMgr gCfgMgr;
