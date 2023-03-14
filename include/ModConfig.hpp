#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    CONFIG_VALUE(PlatformHeightOffsetMeters,   float, "platformHeightOffsetMeters",   -0.34);
    CONFIG_VALUE(PlatformDistanceOffsetMeters, float, "platformDistanceOffsetMeters", -0.25);

    CONFIG_VALUE(AutoPause, bool, "AutoPause", true);

    CONFIG_VALUE(OtherHandClicks, bool, "OtherHandClicks", false);
    CONFIG_VALUE(ModEnabled, bool, "ModEnabled", true);
    CONFIG_VALUE(HandMode, bool, "HandMode", false);
    CONFIG_VALUE(LeftHandTargetIdx,  int, "LeftHandTargetIdx",  1);
    CONFIG_VALUE(RightHandTargetIdx, int, "RightHandTargetIdx", 1);
)