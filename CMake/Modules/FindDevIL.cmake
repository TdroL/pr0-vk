# Find DevIL
#
# DEVIL_INCLUDE_DIR
# DEVIL_LIBRARIES
# DEVIL_FOUND

find_path(DEVIL_INCLUDE_DIR NAMES IL/il.h HINTS
	"${CMAKE_SOURCE_DIR}/external/devil-${DevIL_FIND_VERSION}/include")

find_library(DEVIL_LIBRARY NAMES DevIL.lib DevIL HINTS
	"${CMAKE_SOURCE_DIR}/external/devil-${DevIL_FIND_VERSION}/lib/x64/Release")

find_library(ILU_LIBRARY NAMES ILU.lib ILU HINTS
	"${CMAKE_SOURCE_DIR}/external/devil-${DevIL_FIND_VERSION}/lib/x64/Release")

find_library(ILUT_LIBRARY NAMES ILUT.lib ILUT HINTS
	"${CMAKE_SOURCE_DIR}/external/devil-${DevIL_FIND_VERSION}/lib/x64/Release")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DevIL DEFAULT_MSG DEVIL_LIBRARY ILU_LIBRARY ILUT_LIBRARY DEVIL_INCLUDE_DIR)

set(DEVIL_LIBRARIES ${DEVIL_LIBRARY} ${ILU_LIBRARY} ${ILUT_LIBRARY})

mark_as_advanced(DEVIL_INCLUDE_DIR DEVIL_LIBRARIES)