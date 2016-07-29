cmake_minimum_required(VERSION 3.2)

set(NAME pr0-vk)

project(${NAME})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
	"${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_definitions("-Wall -Wextra -Wfatal-errors -std=c++14 -m64")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

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

# Vulkan
include_directories($ENV{VK_SDK_PATH}/Include)
link_directories($ENV{VK_SDK_PATH}/Source/lib)
find_library(VULKAN_1_LIB NAMES vulkan-1)
message(STATUS "VULKAN: ${VULKAN_1_LIB}")
set(LIBS ${LIBS} ${VULKAN_1_LIB})

# Vulkan.hpp
include_directories(${CMAKE_SOURCE_DIR}/external/vulkan-hpp)

# GLM
include_directories(${CMAKE_SOURCE_DIR}/external/glm)

# GLFW
include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include)
find_library(GLFW_LIB NAMES glfw3 glfw glfw3dll glfwdll PATHS ${CMAKE_SOURCE_DIR}/external/glfw/lib-mingw-w64)
message(STATUS "GLFW3: ${GLFW_LIB}")
set(LIBS ${LIBS} ${GLFW_LIB})

add_executable(${NAME} ${SOURCES})
target_link_libraries(${NAME} ${LIBS})