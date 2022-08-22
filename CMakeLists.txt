project (eli_extra_utils)

file(GLOB eli_extra_utils_sources ./src/**.c)
set(eli_extra_utils ${eli_extra_utils_sources})

add_library(eli_extra_utils ${eli_extra_utils})
target_link_libraries(eli_extra_utils)