# Find Spdlog
#
# SPDLOG_INCLUDE_DIR
# SPDLOG_FOUND

find_path(SPDLOG_INCLUDE_DIR NAMES spdlog/spdlog.h HINTS
	"$ENV{SPDLOG_SDK}"
	"${CMAKE_SOURCE_DIR}/external/spdlog-${Spdlog_FIND_VERSION}/include")


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Spdlog DEFAULT_MSG SPDLOG_INCLUDE_DIR)

mark_as_advanced(SPDLOG_INCLUDE_DIR)