#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    CONFIG_VALUE(PlatformHeightOffsetMeters,   float, "platformHeightOffsetMeters",   -0.34);
    CONFIG_VALUE(PlatformDistanceOffsetMeters, float, "platformDistanceOffsetMeters", -0.25);
      
    CONFIG_VALUE(OtherHandClicks, bool, "OtherHandClicks", false);
    CONFIG_VALUE(ModEnabled, bool, "ModEnabled", true);
    CONFIG_VALUE(HandMode, bool, "HandMode", false);
    CONFIG_VALUE(LeftHandTargetIdx,  int, "LeftHandTargetIdx",  1);
    CONFIG_VALUE(RightHandTargetIdx, int, "RightHandTargetIdx", 1);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(PlatformHeightOffsetMeters);
        CONFIG_INIT_VALUE(PlatformDistanceOffsetMeters);

        CONFIG_INIT_VALUE(OtherHandClicks);
        CONFIG_INIT_VALUE(HandMode);
        CONFIG_INIT_VALUE(ModEnabled);
        CONFIG_INIT_VALUE(LeftHandTargetIdx);
        CONFIG_INIT_VALUE(RightHandTargetIdx);
    )
);