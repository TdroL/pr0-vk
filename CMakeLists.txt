cmake_minimum_required(VERSION 3.2)

set(NAME pr0-vk)

project(${NAME})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Modules")

include(cotire)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	add_definitions(-DDEBUG)
	add_definitions(-DVALIDATE_SYMBOLS)
endif ()

# if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
# 	"${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wfatal-errors -std=c++1z -m64 -pthread -fdiagnostics-color")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -Og -Weffc++ -Winvalid-pch")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3 -funroll-loops -msse2")

	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m64 -static")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m64 -static")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64 -static")
elseif (CMAKE_CL_64)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /std:c++latest /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /std:c++latest /MTd")
else ()
	message(FATAL_ERROR "Unsupported compiler ${CMAKE_CXX_COMPILER_ID}")
endif ()

# Project files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.hpp")

set(INCLUDE_DIRS "")
foreach(_headerFile ${HEADERS})
	get_filename_component(_dir ${_headerFile} PATH)
	list(APPEND INCLUDE_DIRS ${_dir})
endforeach()

list(REMOVE_DUPLICATES INCLUDE_DIRS)
include_directories(${INCLUDE_DIRS})

# GLFW
find_package(GLFW 3.2.1 REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})
list(APPEND LIBS ${GLFW_LIBRARY})

# Vulkan
find_package(Vulkan REQUIRED)
include_directories(${VULKAN_INCLUDE_DIR})
list(APPEND LIBS ${VULKAN_LIBRARIES})

# GLM
find_package(GLM 0.9.8.5 REQUIRED)
include_directories(${GLM_INCLUDE_DIR})

# JSON
find_package(JSON 2.1.1 REQUIRED)
include_directories(${JSON_INCLUDE_DIR})

# Spdlog
find_package(Spdlog 0.14.0 REQUIRED)
include_directories(${SPDLOG_INCLUDE_DIR})

# Remotery
find_package(Remotery 17.10.03 REQUIRED)
include_directories(${REMOTERY_INCLUDE_DIR})
list(APPEND LIBS ${REMOTERY_LIBRARIES})

# executable
add_executable(${NAME} ${SOURCES})
target_link_libraries(${NAME} ${LIBS})
cotire(${NAME})