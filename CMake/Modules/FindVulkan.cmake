# Find Vulkan
#
# VULKAN_INCLUDE_DIR
# VULKAN_LIBRARY
# VULKAN_FOUND

if (WIN32)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(VULKAN_LIBRARY_PATH
			"$ENV{VULKAN_SDK}/Source/lib"
			"$ENV{VK_SDK_PATH}/Source/lib")
	else ()
		set(VULKAN_LIBRARY_PATH
			"$ENV{VULKAN_SDK}/Bin"
			"$ENV{VK_SDK_PATH}/Bin")
	endif ()

	find_path(VULKAN_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS
		"$ENV{VULKAN_SDK}/Include"
		"$ENV{VK_SDK_PATH}/Include")
	find_library(VULKAN_LIBRARY NAMES vulkan-1.lib HINTS ${VULKAN_LIBRARY_PATH})
else ()
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(VULKAN_LIBRARY_PATH
			"$ENV{VULKAN_SDK}/Source/lib")
	else ()
		set(VULKAN_LIBRARY_PATH
			"$ENV{VULKAN_SDK}/lib")
	endif ()

	find_path(VULKAN_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS
		"$ENV{VULKAN_SDK}/include")
	find_library(VULKAN_LIBRARY NAMES vulkan-1 HINTS ${VULKAN_LIBRARY_PATH})
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vulkan DEFAULT_MSG VULKAN_LIBRARY VULKAN_INCLUDE_DIR)

set(VULKAN_LIBRARIES ${VULKAN_LIBRARY})

mark_as_advanced(VULKAN_INCLUDE_DIR VULKAN_LIBRARIES)