# Find span-lite
#
# SPANLITE_INCLUDE_DIR
# SPANLITE_FOUND

find_path(SPANLITE_INCLUDE_DIR NAMES nonstd/span.hpp HINTS
	"$ENV{SPANLITE_SDK}"
	"${CMAKE_SOURCE_DIR}/external/span-lite-${SpanLite_FIND_VERSION}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SpanLite DEFAULT_MSG SPANLITE_INCLUDE_DIR)

mark_as_advanced(SPANLITE_INCLUDE_DIR)