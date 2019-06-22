# Find Draco
#
# DRACO_INCLUDE_DIRS
# DRACO_LIBRARIES
# DRACO_FOUND

find_path(DRACO_INCLUDE_DIR NAMES draco/draco_features.h HINTS
	"${CMAKE_SOURCE_DIR}/external/draco-${Draco_FIND_VERSION}/include")

find_library(DRACO_LIBRARY NAMES draco HINTS
	"${CMAKE_SOURCE_DIR}/external/draco-${Draco_FIND_VERSION}/lib")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Draco DEFAULT_MSG DRACO_LIBRARY DRACO_INCLUDE_DIR)

set(DRACO_LIBRARIES ${DRACO_LIBRARY})

mark_as_advanced(DRACO_INCLUDE_DIRS DRACO_LIBRARIES)