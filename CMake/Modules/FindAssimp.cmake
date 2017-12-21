# Find Assimp
#
# ASSIMP_INCLUDE_DIR
# ASSIMP_LIBRARIES
# ASSIMP_FOUND

find_path(ASSIMP_INCLUDE_DIR NAMES assimp/anim.h HINTS
	"$ENV{ASSIMP_SDK}/include"
	"${CMAKE_SOURCE_DIR}/external/assimp-${Assimp_FIND_VERSION}/include")

find_library(ASSIMP_LIBRARY NAMES assimp assimp.dll HINTS
	"$ENV{ASSIMP_SDK}/lib"
	"${CMAKE_SOURCE_DIR}/external/assimp-${Assimp_FIND_VERSION}/lib")

find_library(IRRXML_LIBRARY NAMES IrrXML HINTS
	"$ENV{ASSIMP_SDK}/lib"
	"${CMAKE_SOURCE_DIR}/external/assimp-${Assimp_FIND_VERSION}/lib")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Assimp DEFAULT_MSG ASSIMP_LIBRARY IRRXML_LIBRARY ASSIMP_INCLUDE_DIR)

set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY} ${IRRXML_LIBRARY})

mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARIES)
