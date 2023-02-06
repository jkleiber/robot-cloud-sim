
#pragma once

typedef struct boat_state_t
{
    double lat;
    double lon;
    double yaw;
} BoatState;

typedef struct boat_sim_info_t
{
    double dt;
} BoatSimInfo;

typedef struct boat_data_t
{
    BoatState state;

    BoatSimInfo sim_info;

} BoatData;