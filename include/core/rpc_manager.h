
#pragma once

#include <memory>
#include <thread>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "core/rpc_data.h"

template <class T> class RpcManager
{
public:
    RpcManager(std::shared_ptr<T> service, std::string addr = "0.0.0.0:9002")
        : service_(service), addr_(addr)
    {
    }
    ~RpcManager()
    {
        // Shut the server down if it exists
        if (server_ != nullptr)
        {
            server_->Shutdown();
        }
    }

    bool Init()
    {
        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();

        // Listen on the given address without any authentication mechanism.
        builder_.AddListeningPort(addr_, grpc::InsecureServerCredentials());
        // Register "service" as the instance through which we'll communicate
        // with clients. In this case it corresponds to an *synchronous*
        // service.
        builder_.RegisterService(service_.get());

        // Add a completion queue
        // server_cq_ = builder.AddCompletionQueue();

        // Finally assemble the server.
        std::unique_ptr<grpc::Server> tmp(builder_.BuildAndStart());
        server_ = std::move(tmp);
        std::cout << "Server listening on " << addr_ << std::endl;

        return true;
    }

    bool Run()
    {
        VERIFY(server_ != nullptr);
        // Wait for the server to shutdown. Note that some other thread must be
        // responsible for shutting down the server for this call to ever
        // return.
        server_->Wait();

        return true;
    }

private:
    std::shared_ptr<T> service_;
    std::unique_ptr<grpc::Server> server_ = nullptr;
    // grpc::ServerCompletionQueue server_cq_;
    grpc::ServerBuilder builder_;
    std::string addr_;
};
