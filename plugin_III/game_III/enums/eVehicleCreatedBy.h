/*
    Plugin-SDK (Grand Theft 3) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once
#include <cstdint>

enum eVehicleCreatedBy : int8_t {
    RANDOM_VEHICLE = 1,
    MISSION_VEHICLE,
    PARKED_VEHICLE,
    PERMANENT_VEHICLE
};