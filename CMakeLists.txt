project (eli_extra_utils)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB eli_extra_utils_sources ./src/**.c)
set(eli_extra_utils ${eli_extra_utils_sources})

add_library(eli_extra_utils ${eli_extra_utils})
target_link_libraries(eli_extra_utils)