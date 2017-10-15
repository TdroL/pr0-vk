# Find GLFW
#
# GLFW_INCLUDE_DIR
# GLFW_LIBRARY
# GLFW_FOUND

if (WIN32)
	find_path(GLFW_INCLUDE_DIR NAMES GLFW/glfw3.h HINTS
		"$ENV{GLFW_SDK}/include"
		"${CMAKE_SOURCE_DIR}/external/glfw-${GLFW_FIND_VERSION}/include")

	if (CMAKE_CL_64)
		find_library(GLFW_LIBRARY NAMES glfw3 HINTS
			"$ENV{GLFW_SDK}/lib-vc2015"
			"${CMAKE_SOURCE_DIR}/external/glfw-${GLFW_FIND_VERSION}/lib-vc2015")
	else ()
		find_library(GLFW_LIBRARY NAMES glfw3 HINTS
			"$ENV{GLFW_SDK}/lib-mingw-w64"
			"${CMAKE_SOURCE_DIR}/external/glfw-${GLFW_FIND_VERSION}/lib-mingw-w64")
	endif ()
else ()
	find_path(GLFW_INCLUDE_DIR NAMES GLFW/glfw3.h HINTS
		"$ENV{GLFW_SDK}/include"
		"${CMAKE_SOURCE_DIR}/external/glfw-${GLFW_FIND_VERSION}/include")

	find_library(GLFW_LIBRARY NAMES glfw3 HINTS
		"$ENV{GLFW_SDK}/lib"
		"${CMAKE_SOURCE_DIR}/external/glfw-${GLFW_FIND_VERSION}/lib")
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_LIBRARY GLFW_INCLUDE_DIR)

mark_as_advanced(GLFW_INCLUDE_DIR GLFW_LIBRARY)