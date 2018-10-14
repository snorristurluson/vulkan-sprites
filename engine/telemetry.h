//
// Created by snorri on 14.10.2018.
//

#ifndef VULKAN_SPRITES_TELEMETRY_H
#define VULKAN_SPRITES_TELEMETRY_H

#if TELEMETRY_ENABLED
#include "rad_tm.h"
#else
#define tmFunction(...)
#define tmTick(...)
#endif

bool ConnectTelemetry();

#endif //VULKAN_SPRITES_TELEMETRY_H
