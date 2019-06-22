# Find Remotery
#
# REMOTERY_INCLUDE_DIR
# REMOTERY_LIBRARIES
# REMOTERY_FOUND

if (WIN32)
	find_path(REMOTERY_INCLUDE_DIR NAMES Remotery.h HINTS
		"${CMAKE_SOURCE_DIR}/external/remotery-${Remotery_FIND_VERSION}/lib")

	find_library(REMOTERY_LIBRARY NAMES Remotery HINTS
		"${CMAKE_SOURCE_DIR}/external/remotery-${Remotery_FIND_VERSION}/lib")

	find_library(WS2_32_LIBRARY NAMES ws2_32)

	set(REMOTERY_LIBRARIES ${REMOTERY_LIBRARY} ${WS2_32_LIBRARY})
else ()
	find_path(REMOTERY_INCLUDE_DIR NAMES Remotery.h HINTS
		"${CMAKE_SOURCE_DIR}/external/remotery-${Remotery_FIND_VERSION}/lib")

	find_library(REMOTERY_LIBRARY NAMES Remotery HINTS
		"${CMAKE_SOURCE_DIR}/external/remotery-${Remotery_FIND_VERSION}/lib")

	set(REMOTERY_LIBRARIES ${REMOTERY_LIBRARY})
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Remotery DEFAULT_MSG REMOTERY_LIBRARY REMOTERY_INCLUDE_DIR)

mark_as_advanced(REMOTERY_INCLUDE_DIR REMOTERY_LIBRARIES)