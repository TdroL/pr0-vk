# Find JSON
#
# JSON_INCLUDE_DIR
# JSON_FOUND

find_path(JSON_INCLUDE_DIR NAMES json.hpp HINTS
	"$ENV{JSON_SDK}"
	"${CMAKE_SOURCE_DIR}/external/json-${JSON_FIND_VERSION}/src")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JSON DEFAULT_MSG JSON_INCLUDE_DIR)

mark_as_advanced(JSON_INCLUDE_DIR)