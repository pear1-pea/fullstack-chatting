#include "LogicSystem.h"
#include "HttpConnection.h"
#include "const.h"
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/http/fields_fwd.hpp>

LogicSystem::~LogicSystem() {
}

void LogicSystem::RegGet(std::string url, HttpHandler handler){
    _get_handlers.insert(std::make_pair(url,handler));
}//一个url对应一个处理器

LogicSystem::LogicSystem(){
    RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
        beast::ostream(connection->_response.body()) << "receive get_test req " << std::endl;
        int i = 0;
        for (auto& elem : connection->_get_params) {
            i++;
            beast::ostream(connection->_response.body()) << "param" << i << " key is " << elem.first;
            beast::ostream(connection->_response.body()) << ", " <<  " value is " << elem.second << std::endl;
        }
    });
    //处理post请求
    RegPost("/get_varifycode",[](std::shared_ptr<HttpConnection> connection){
        auto body_str=boost::beast::buffers_to_string(connection->_request.body().data());
        std::cout<<"receive body is "<<body_str<<std::endl;
        connection->_response.set(http::field::content_type,"text/json");
        Json::Value root;
        Json::Reader reader;
        Json::Value src_root;
        bool parse_success = reader.parse(body_str,src_root);
        if(!parse_success){
            std::cout<<"Failed to parse JSON data!"<<std::endl;
            root["error"]=ErrorCodes::Error_Json;
            Json::StreamWriterBuilder writer;
            std::string jsonstr = Json::writeString(writer, root);
            beast::ostream(connection->_response.body())<<jsonstr;
            return true;
        }
        auto email = src_root["email"].asString();
        std::cout<<"email is "<<email<<std::endl;
        root["error"]=0;
        root["email"]=src_root["email"];
        Json::StreamWriterBuilder writer;
        std::string jsonstr = Json::writeString(writer, root);
        beast::ostream(connection->_response.body())<<jsonstr;
        return true;
    });
}

bool LogicSystem::HandleGet(std::string path,std::shared_ptr<HttpConnection> con){
    if(_get_handlers.find(path)==_get_handlers.end()){
        return false;
    }
    _get_handlers[path](con);//调用对应的处理器
    return true;
}

void LogicSystem::RegPost(std::string url, HttpHandler handler) {
    _post_handlers.insert(std::make_pair(url,handler));
}

bool LogicSystem::HandlePost(std::string path,std::shared_ptr<HttpConnection> con){
    if(_post_handlers.find(path)==_post_handlers.end()){
        return false;
    }
    _post_handlers[path](con);
    return true;
}


