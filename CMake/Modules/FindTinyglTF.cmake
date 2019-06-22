# Find TinyglTF
#
# TINYGLTF_INCLUDE_DIR
# TINYGLTF_FOUND

find_path(TINYGLTF_INCLUDE_DIR NAMES tiny_gltf.h HINTS
	"$ENV{TINYGLTF_SDK}"
	"${CMAKE_SOURCE_DIR}/external/tinygltf-${TinyglTF_FIND_VERSION}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TinyglTF DEFAULT_MSG TINYGLTF_INCLUDE_DIR)

mark_as_advanced(TINYGLTF_INCLUDE_DIR)