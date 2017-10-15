# Find GLM
#
# GLM_INCLUDE_DIR
# GLM_FOUND

find_path(GLM_INCLUDE_DIR NAMES glm/glm.hpp HINTS
	"$ENV{GLM_SDK}"
	"${CMAKE_SOURCE_DIR}/external/glm-${GLM_FIND_VERSION}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)

mark_as_advanced(GLM_INCLUDE_DIR)