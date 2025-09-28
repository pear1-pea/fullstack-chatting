#ifndef VARIFYGRPCCLIENT_H
#define VARIFYGRPCCLIENT_H

#include "../generated/message.grpc.pb.h"
#include "../generated/message.pb.h"
#include "Singleton.h"
#include "const.h"
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class VarifyGrpcClient : public Singleton<VarifyGrpcClient> {
  friend class Singleton<VarifyGrpcClient>;

public:
  GetVarifyRsp GetVarifyCode(std::string email);

private:
  std::unique_ptr<VarifyService::Stub> stub_;
  VarifyGrpcClient();
};

#endif // VARIFYGRPCCLIENT_H
