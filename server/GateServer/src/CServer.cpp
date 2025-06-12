#include "CServer.h"


CServer::CServer(boost::asio::io_context& ioc,unsigned short& port):
    _ioc(ioc),
    _acceptor(ioc,tcp::endpoint(tcp::v4(),port)),
    _socket(ioc)
{}

void CServer::Start(){
    auto self = shared_from_this();
    _acceptor.async_accept(_socket,[self](beast::error_code ec){//延长self周期
        try{
            //出错放弃这链接，继续监听其他链接
            if(ec){
                self->Start();
                return;
            } 
            //创建链接管理类，处理socket的读写事件。
            //处理当前链接
            std::make_shared<HttpConnection>(std::move(self->_socket))->Start();//socket无法拷贝，调用移动构造。
            //继续监听新链接
            self->Start();
        }catch(std::exception& exp){

        }
    }
    );
}
