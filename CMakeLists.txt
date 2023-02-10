cmake_minimum_required(VERSION 3.15...3.25)

set(PROJECT_NAME main)

project(${PROJECT_NAME}
  VERSION 0.0.0
  DESCRIPTION ${PROJECT_NAME}
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -Wodr -Wall -Wextra -Wconversion -Wfatal-errors -m64 -pthread -fdiagnostics-color")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Og -ggdb")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -funroll-loops -msse2")

	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m64 -static")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m64 -static")
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} -m64 -static")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} -m64 -static -flto")
elseif (CMAKE_CL_64)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
else ()
	message(FATAL_ERROR "Unsupported compiler ${CMAKE_CXX_COMPILER_ID}")
endif ()

message("Add vulkan")
find_package(Vulkan REQUIRED COMPONENTS glslc)
if (Vulkan_FOUND)
	message(STATUS ${Vulkan_LIBRARY})
else ()
	message(FATAL_ERROR "Could not find Vulkan library")
endif ()

find_program(glslc NAMES glslc HINTS Vulkan::glslc)

include_directories(${Vulkan_INCLUDE_DIR})
add_definitions(-DVULKAN_HPP_NO_CONSTRUCTORS -DVK_NO_PROTOTYPES -DVULKAN_HPP_NO_SETTERS)

message("Add VMA")
option(VMA_STATIC_VULKAN_FUNCTIONS "" OFF)
option(VMA_DYNAMIC_VULKAN_FUNCTIONS "" ON)
add_subdirectory(external/vma)

message("Add fmt")
add_subdirectory(external/fmt)

message("Add glfw")
add_subdirectory(external/glfw)

message("Add json")
add_subdirectory(external/json)

message("Add spdlog")
add_subdirectory(external/spdlog)

message("Add tracy")
option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)
option(TRACY_NO_FRAME_IMAGE "" ON)
option(TRACY_NO_VSYNC_CAPTURE "" ON)
option(TRACY_NO_FRAME_IMAGE "" ON)
option(TRACY_NO_CRASH_HANDLER "" ON)
add_subdirectory(external/tracy)

message("Add imgui")
add_library(imgui STATIC)
target_include_directories(
	imgui
	PUBLIC
		external/imgui
)
target_sources(
	imgui
	PRIVATE
		external/imgui/imgui.cpp
		external/imgui/imgui_widgets.cpp
		external/imgui/imgui_tables.cpp
		external/imgui/imgui_draw.cpp
		external/imgui/imgui_demo.cpp
)
target_compile_definitions(
	imgui
	PRIVATE
		-DImTextureID=ImU64
)
target_link_libraries(
	imgui
)

# C++ sources
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")

add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/src/main.cpp")
target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		"${CMAKE_SOURCE_DIR}/src"
)
target_sources(
	${PROJECT_NAME}
	PRIVATE
		${SOURCES}
)
target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		Vulkan::Headers
		VulkanMemoryAllocator
		fmt::fmt
		glfw
		nlohmann_json::nlohmann_json
		spdlog::spdlog
		Tracy::TracyClient
		imgui
)

# shader sources
file(
	GLOB_RECURSE SHADER_SOURCES
	"${CMAKE_SOURCE_DIR}/shaders/*.vert"
	"${CMAKE_SOURCE_DIR}/shaders/*.frag"
	"${CMAKE_SOURCE_DIR}/shaders/*.comp"
	"${CMAKE_SOURCE_DIR}/shaders/*.geom"
	"${CMAKE_SOURCE_DIR}/shaders/*.tesc"
	"${CMAKE_SOURCE_DIR}/shaders/*.tese"
	"${CMAKE_SOURCE_DIR}/shaders/*.mesh"
	"${CMAKE_SOURCE_DIR}/shaders/*.task"
	"${CMAKE_SOURCE_DIR}/shaders/*.rgen"
	"${CMAKE_SOURCE_DIR}/shaders/*.rchit"
	"${CMAKE_SOURCE_DIR}/shaders/*.rmiss"
)

foreach(SOURCE ${SHADER_SOURCES})
	get_filename_component(FILE_NAME ${SOURCE} NAME)
	set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")

	add_custom_command(
		OUTPUT ${SPIRV}
		DEPENDS ${SOURCE}
		COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
		COMMAND ${glslc} --target-env=vulkan1.3 -Werror -g -o ${SPIRV} ${SOURCE}
	)
	list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach()

add_custom_target(Shaders DEPENDS ${SPIRV_BINARY_FILES})
add_dependencies(${PROJECT_NAME} Shaders)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_BINARY_DIR}/shaders"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
)