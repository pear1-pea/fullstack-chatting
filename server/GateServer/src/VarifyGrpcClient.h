#ifndef VARIFYGRPCCLIENT_H
#define VARIFYGRPCCLIENT_H

#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"
#include "message.pb.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;


class VarifyGrpcClient : public Singleton<VarifyGrpcClient>
{
    friend class Singleton<VarifyGrpcClient>;
public:
    GetVarifyRsp GetVarifyCode(std::string email);
private:
    std::unique_ptr<VarifyService::Stub> stub_;
    VarifyGrpcClient();
};

#endif // VARIFYGRPCCLIENT_H