#pragma once

#define CONFIG_PROFILE_FULL 1
#define CONFIG_PROFILE_SMALL 2
#define CONFIG_PROFILE_TINY 3


#define IS_PROFILE(profile) (CONFIG_PROFILE == CONFIG_PROFILE_##profile)
