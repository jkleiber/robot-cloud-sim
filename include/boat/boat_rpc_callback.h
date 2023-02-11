
#pragma once

#include <string.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "msg/boat_rpc.grpc.pb.h"

using grpc::CallbackServerContext;
using grpc::ServerUnaryReactor;

class BoatServiceCb final : public msg::BoatService::CallbackService
{
public:
    BoatServiceCb(std::string *name) : name_(name) {}

    ServerUnaryReactor *SayHello(CallbackServerContext *context,
                                 const msg::BoatRequest *request,
                                 msg::BoatResponse *reply) override
    {
        std::string prefix("Hello ");
        reply->set_message(prefix + request->name());

        // Update name.
        if (name_ != nullptr)
        {
            *name_ = request->name();
        }

        ServerUnaryReactor *reactor = context->DefaultReactor();
        reactor->Finish(grpc::Status::OK);
        return reactor;
    }

private:
    std::string *const name_ = nullptr;
};