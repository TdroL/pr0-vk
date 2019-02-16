# Find DevIL
#
# STB_IMAGE_INCLUDE_DIR
# STB_IMAGE_LIBRARIES
# STB_IMAGE_FOUND

find_path(STB_IMAGE_INCLUDE_DIR NAMES stb/stb_image.h HINTS
	"${CMAKE_SOURCE_DIR}/external/stb_image-${Stb_image_FIND_VERSION}/include")

find_library(STB_IMAGE_LIBRARY NAMES stb_image HINTS
	"${CMAKE_SOURCE_DIR}/external/stb_image-${Stb_image_FIND_VERSION}/lib")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DevIL DEFAULT_MSG STB_IMAGE_LIBRARY STB_IMAGE_INCLUDE_DIR)

set(STB_IMAGE_LIBRARIES ${STB_IMAGE_LIBRARY})

mark_as_advanced(STB_IMAGE_INCLUDE_DIR STB_IMAGE_LIBRARIES)