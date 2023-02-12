
#pragma once

#include <string.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "boat/boat_data.h"
#include "boat/boat_msg.grpc.pb.h"

using grpc::CallbackServerContext;
using grpc::ServerUnaryReactor;

class BoatServiceCb final : public msg::BoatService::CallbackService
{
public:
    BoatServiceCb(std::string *name, BoatControl *boat_ctrl, const double &t,
                  const BoatState &boat_state)
        : name_(name), ctrl_(boat_ctrl), t_(t), state_(boat_state)
    {
    }

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

    ServerUnaryReactor *ControlBoat(CallbackServerContext *context,
                                    const msg::BoatControl *ctrl,
                                    msg::BoatState *state) override
    {
        // Receive the control and set the control pointer values.
        ctrl_->power = ctrl->power();
        ctrl_->rudder = ctrl->rudder();

        // Respond with the current boat state.
        state->set_t(t_);
        state->set_lat(state_.lat);
        state->set_lon(state_.lon);
        state->set_yaw(state_.yaw);

        ServerUnaryReactor *reactor = context->DefaultReactor();
        reactor->Finish(grpc::Status::OK);
        return reactor;
    }

    ServerUnaryReactor *GetState(CallbackServerContext *context,
                                 const msg::BoatRequest *req,
                                 msg::BoatState *state) override
    {
        // Respond with the current boat state.
        state->set_t(t_);
        state->set_lat(state_.lat);
        state->set_lon(state_.lon);
        state->set_yaw(state_.yaw);

        ServerUnaryReactor *reactor = context->DefaultReactor();
        reactor->Finish(grpc::Status::OK);
        return reactor;
    }

private:
    std::string *const name_ = nullptr;

    BoatControl *const ctrl_;
    const BoatState &state_;
    const double &t_;
};