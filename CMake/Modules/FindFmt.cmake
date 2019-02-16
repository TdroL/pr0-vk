# Find Fmt
#
# FMT_INCLUDE_DIR
# FMT_FOUND

find_path(FMT_INCLUDE_DIR NAMES fmt/core.h HINTS
	"$ENV{FMT_SDK}"
	"${CMAKE_SOURCE_DIR}/external/fmt-${Fmt_FIND_VERSION}/include")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Fmt DEFAULT_MSG FMT_INCLUDE_DIR)

mark_as_advanced(FMT_INCLUDE_DIR)