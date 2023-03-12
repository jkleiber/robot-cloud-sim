
#pragma once

#include <string.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "auv/auv_data.h"
#include "auv/auv_msg.grpc.pb.h"
#include "core/time.h"

using grpc::CallbackServerContext;
using grpc::ServerUnaryReactor;

class AUVServiceCb final : public msg::AUVService::CallbackService
{
public:
    AUVServiceCb(std::string *name, AUVControl *auv_ctrl, const double &t,
                 const AUVState &auv_state)
        : name_(name), ctrl_(auv_ctrl), t_(t), state_(auv_state)
    {
    }

    ServerUnaryReactor *SayHello(CallbackServerContext *context,
                                 const msg::AUVRequest *request,
                                 msg::AUVResponse *reply) override
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

    ServerUnaryReactor *ControlAUV(CallbackServerContext *context,
                                   const msg::AUVControl *ctrl,
                                   msg::AUVState *state) override
    {
        // Receive the control and set the control pointer values.
        ctrl_->power = ctrl->power();
        ctrl_->rudder = ctrl->rudder();

        // Make time a double
        double t = static_cast<double>(t_) / static_cast<double>(kSec);

        // Respond with the current auv state.
        state->set_t(t);
        state->set_lat(state_.lat);
        state->set_lon(state_.lon);
        state->set_yaw(state_.yaw);

        ServerUnaryReactor *reactor = context->DefaultReactor();
        reactor->Finish(grpc::Status::OK);
        return reactor;
    }

    ServerUnaryReactor *GetState(CallbackServerContext *context,
                                 const msg::AUVRequest *req,
                                 msg::AUVState *state) override
    {

        // Make time a double
        double t = static_cast<double>(t_) / static_cast<double>(kSec);

        // Respond with the current auv state.
        state->set_t(t);
        state->set_lat(state_.lat);
        state->set_lon(state_.lon);
        state->set_yaw(state_.yaw);

        ServerUnaryReactor *reactor = context->DefaultReactor();
        reactor->Finish(grpc::Status::OK);
        return reactor;
    }

private:
    std::string *const name_ = nullptr;

    AUVControl *const ctrl_;
    const AUVState &state_;
    const double &t_;
};