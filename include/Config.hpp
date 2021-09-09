#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,
    CONFIG_VALUE(SomeKindOfFloat, float, "SomeKindOfFloat", 1.0);
    
    CONFIG_VALUE(ModEnabled, bool, "ModEnabled", true);
    CONFIG_VALUE(HandMode, bool, "HandMode", false);
    CONFIG_VALUE(LeftHandTargetIdx,  int, "LeftHandTargetIdx",  0);
    CONFIG_VALUE(RightHandTargetIdx, int, "RightHandTargetIdx", 0);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(SomeKindOfFloat);

        CONFIG_INIT_VALUE(HandMode);
        CONFIG_INIT_VALUE(ModEnabled);
        CONFIG_INIT_VALUE(LeftHandTargetIdx);
        CONFIG_INIT_VALUE(RightHandTargetIdx);
    )
);