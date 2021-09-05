#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,
    CONFIG_VALUE(SomeKindOfFloat, float, "SomeKindOfFloat", 1.0);
    CONFIG_VALUE(IntegerValueThind, int, "IntegerValueThind", 32);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(SomeKindOfFloat);
        CONFIG_INIT_VALUE(IntegerValueThind);
    )
);