
#pragma once

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "core/rpc_data.h"

template <class T> class RpcManager
{
public:
    RpcManager(std::string addr = "0.0.0.0:9002") : addr_(addr) {}
    ~RpcManager()
    {
        VERIFY(server_ != nullptr);
        // Shut the server down if this object is deallocated.
        server_->Shutdown();
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
        builder_.RegisterService(&service_);

        // Finally assemble the server.
        std::unique_ptr<grpc::Server> tmp(builder_.BuildAndStart());
        server_ = std::move(tmp);
        std::cout << "Server listening on " << addr_ << std::endl;

        return true;
    }

    // This should be run in another thread in order to allow the rest of the
    // program to run.
    bool Run()
    {
        VERIFY(server_ != nullptr);
        // Wait for the server to shutdown. Note that some other thread must be
        // responsible for shutting down the server for this call to ever
        // return.
        server_->Wait();

        return true;
    }

    bool AsyncRun()
    {
        std::thread run_thread([&] { this->Run(); });

        return true;
    }

private:
    T service_;
    std::unique_ptr<grpc::Server> server_;
    grpc::ServerBuilder builder_;
    std::string addr_;
};
