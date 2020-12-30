cmake_minimum_required (VERSION 2.6)
project (eli_extra_utils)

file(GLOB eli_extra_utils_sources ./src/**.c)
SET(eli_extra_utils ${eli_extra_utils_sources})

ADD_LIBRARY (eli_extra_utils ${eli_extra_utils})
TARGET_LINK_LIBRARIES (eli_extra_utils)