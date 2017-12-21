# Find GLI
#
# GLI_INCLUDE_DIR
# GLI_FOUND

find_path(GLI_INCLUDE_DIR NAMES gli/gli.hpp HINTS
	"$ENV{GLI_SDK}"
	"${CMAKE_SOURCE_DIR}/external/gli-${GLI_FIND_VERSION}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLI DEFAULT_MSG GLI_INCLUDE_DIR)

mark_as_advanced(GLI_INCLUDE_DIR)