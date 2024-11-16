# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_gain_sch_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED gain_sch_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(gain_sch_FOUND FALSE)
  elseif(NOT gain_sch_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(gain_sch_FOUND FALSE)
  endif()
  return()
endif()
set(_gain_sch_CONFIG_INCLUDED TRUE)

# output package information
if(NOT gain_sch_FIND_QUIETLY)
  message(STATUS "Found gain_sch: 0.0.0 (${gain_sch_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'gain_sch' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${gain_sch_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(gain_sch_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${gain_sch_DIR}/${_extra}")
endforeach()
