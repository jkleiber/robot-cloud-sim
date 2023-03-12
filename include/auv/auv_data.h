
#pragma once

#include <string.h>

typedef struct auv_state_t
{
    double lat = 27.698153;
    double lon = -141.785643;
    double yaw = -110.0;
    double speed = 0.0;
    double prop_rpm = 0.0;
} AUVState;

typedef struct auv_sim_info_t
{
    unsigned long long dt;
} AUVSimInfo;

typedef struct auv_info_t
{
    std::string name = "";
} AUVInfo;

typedef struct auv_ctrl_t
{
    double power = 0.0;
    double rudder = 0.0;
    double elevator = 0.0;
} AUVControl;

typedef struct auv_data_t
{
    AUVState state;

    AUVSimInfo sim_info;

    AUVControl ctrl;

} AUVData;
