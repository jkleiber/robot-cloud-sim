
#pragma once

typedef struct boat_state_t
{
    double lat = 0.0;
    double lon = 0.0;
    double yaw = 0.0;
} BoatState;

typedef struct boat_sim_info_t
{
    double dt;
} BoatSimInfo;

typedef struct boat_info_t
{
    std::string name = "";
} BoatInfo;

typedef struct boat_ctrl_t
{
    double power = 0.0;
    double rudder = 0.0;
} BoatControl;

typedef struct boat_data_t
{
    BoatState state;

    BoatSimInfo sim_info;

    BoatControl ctrl;

} BoatData;