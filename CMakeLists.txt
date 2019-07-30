cmake_minimum_required(VERSION 3.12)

project(pr0-vk)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Modules")

include(cotire)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	add_definitions(-DDEBUG)
	add_definitions(-DVALIDATE_SYMBOLS)
endif ()

# if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
# 	"${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -Wodr -Wall -Wextra -Wconversion -Wfatal-errors -m64 -pthread -fdiagnostics-color -gdwarf-2") # -std=c++2a
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Og")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -funroll-loops -msse2")

	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m64 -static")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m64 -static")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64 -static")
elseif (CMAKE_CL_64)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT") #  /std:c++latest
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd") #  /std:c++latest
else ()
	message(FATAL_ERROR "Unsupported compiler ${CMAKE_CXX_COMPILER_ID}")
endif ()

file(GLOB_RECURSE NGN_SOURCES "${CMAKE_SOURCE_DIR}/src/ngn/*.cpp" "${CMAKE_SOURCE_DIR}/src/ngn/*.hpp")
file(GLOB_RECURSE RN_SOURCES "${CMAKE_SOURCE_DIR}/src/rn/*.cpp" "${CMAKE_SOURCE_DIR}/src/rn/*.hpp")
file(GLOB_RECURSE UTIL_SOURCES "${CMAKE_SOURCE_DIR}/src/util/*.cpp" "${CMAKE_SOURCE_DIR}/src/util/*.hpp")

file(GLOB_RECURSE APP_SOURCES "${CMAKE_SOURCE_DIR}/src/app/*.cpp" "${CMAKE_SOURCE_DIR}/src/app/*.hpp")
list(APPEND APP_SOURCES ${NGN_SOURCES} ${RN_SOURCES} ${UTIL_SOURCES} "${CMAKE_SOURCE_DIR}/src/app.cpp")

# file(GLOB_RECURSE CONV_SOURCES "${CMAKE_SOURCE_DIR}/src/conv/*.cpp" "${CMAKE_SOURCE_DIR}/src/conv/*.hpp")
# list(APPEND CONV_SOURCES ${NGN_SOURCES} ${RN_SOURCES} ${UTIL_SOURCES} "${CMAKE_SOURCE_DIR}/src/conv.cpp")

# file(GLOB_RECURSE PROTO_SOURCES "${CMAKE_SOURCE_DIR}/src/proto/*.cpp" "${CMAKE_SOURCE_DIR}/src/proto/*.hpp")
# list(APPEND PROTO_SOURCES ${NGN_SOURCES} ${RN_SOURCES} ${UTIL_SOURCES} "${CMAKE_SOURCE_DIR}/src/proto.cpp")

# if (CMAKE_COMPILER_IS_GNUCXX)
	# list(APPEND APP_LIBS stdc++fs)
	# list(APPEND CONV_LIBS stdc++fs)
	# list(APPEND PROTO_LIBS stdc++fs)
# endif ()

include_directories("${CMAKE_SOURCE_DIR}/src")

# GLFW
find_package(GLFW 3.2.1 REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})

# Vulkan
find_package(Vulkan REQUIRED)
include_directories(${VULKAN_INCLUDE_DIR})

# GLI
find_package(GLI 0.8.3.0 REQUIRED)
include_directories(${GLI_INCLUDE_DIR})

# Stb image
find_package(Stb_image 2.22 REQUIRED)
include_directories(${STB_IMAGE_INCLUDE_DIR})

# GLM
find_package(GLM 0.9.9.5 REQUIRED)
include_directories(${GLM_INCLUDE_DIR})

# JSON
find_package(JSON 3.6.1 REQUIRED)
include_directories(${JSON_INCLUDE_DIR})

# Spdlog
find_package(Spdlog 1.3.1 REQUIRED)
include_directories(${SPDLOG_INCLUDE_DIR})

# Fmt
find_package(Fmt 5.3.0 REQUIRED)
include_directories(${FMT_INCLUDE_DIR})

# span-lite
# find_package(SpanLite 0.5.0 REQUIRED)
# include_directories(${SPANLITE_INCLUDE_DIR})
# add_definitions(-Dspan_FEATURE_MAKE_SPAN=1)

# Remotery
find_package(Remotery 19.03.26 REQUIRED)
include_directories(${REMOTERY_INCLUDE_DIR})

# Assimp
# find_package(Assimp 4.1.0 REQUIRED)
# include_directories(${ASSIMP_INCLUDE_DIR})

# Draco
find_package(Draco 1.3.5 REQUIRED)
include_directories(${DRACO_INCLUDE_DIR})

# TinyglTF
find_package(TinyglTF 2.2.0 REQUIRED)
include_directories(${TINYGLTF_INCLUDE_DIR})

list(APPEND APP_LIBS ${GLFW_LIBRARY} ${VULKAN_LIBRARIES} ${STB_IMAGE_LIBRARIES} ${DRACO_LIBRARIES} ${REMOTERY_LIBRARIES})
# list(APPEND CONV_LIBS ${GLFW_LIBRARY} ${DRACO_LIBRARIES} ${REMOTERY_LIBRARIES})
# list(APPEND PROTO_LIBS ${GLFW_LIBRARY} ${VULKAN_LIBRARIES} ${STB_IMAGE_LIBRARIES} ${DRACO_LIBRARIES} ${REMOTERY_LIBRARIES})

# executables
add_executable(app ${APP_SOURCES})
target_link_libraries(app ${APP_LIBS})
target_compile_features(app PRIVATE cxx_std_17)
cotire(app)

# add_executable(conv ${CONV_SOURCES})
# target_link_libraries(conv ${CONV_LIBS})
# target_compile_features(conv PRIVATE cxx_std_17)
# cotire(conv)

# add_executable(proto ${PROTO_SOURCES})
# target_link_libraries(proto ${PROTO_LIBS})
# target_compile_features(proto PRIVATE cxx_std_17)
# cotire(proto)

# add_custom_target(deploy)
# set(DEPLOY_LIBS ${APP_LIBS} ${CONV_LIBS})
# list(REMOVE_DUPLICATES DEPLOY_LIBS)
# foreach(LIB ${DEPLOY_LIBS})
# 	if(${LIB} MATCHES "^.+/external/.+\\/([^/]+.dll)$")
# 		string(REGEX REPLACE "^.+/external/.+\\/([^/]+.dll)$" "\\1" LIB_BIN ${LIB})
# 		add_custom_command(TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${LIB} ${PROJECT_BINARY_DIR}/.)
# 	endif()
# endforeach()