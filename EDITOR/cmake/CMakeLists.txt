# ==============================================================================
# Aswin2DEditor - C++ executable CMake configuration
# ==============================================================================

# --- Compiler settings ---
include(${CMAKE_SOURCE_DIR}/cmake/CompilersSettings.cmake)

# --- Set base directories ---
set(ASWIN_EDITOR_BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(ASWIN_EDITOR_LIB_DIR "${CMAKE_BINARY_DIR}/lib")

# --- Detect compiler name ---
if(MSVC)
	set(COMPILER_NAME "MSVC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(COMPILER_NAME "Clang")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(COMPILER_NAME "GCC")
else()
	set(COMPILER_NAME "${CMAKE_CXX_COMPILER_ID}")
endif()

# --- Output directories with compiler name ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ASWIN_EDITOR_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ASWIN_EDITOR_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ASWIN_EDITOR_LIB_DIR}/${COMPILER_NAME}/$<CONFIG>)

# --- Copy assets at build time ---
if(ASWIN_EDITOR_COPY_ASSETS)
	if(EXISTS ${ASWIN_EDITOR_ASSETS_DIR})
		add_custom_target(CopyEditorAssets ALL
			COMMAND ${CMAKE_COMMAND} -E make_directory
				"${ASWIN_EDITOR_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>/assets"
			COMMAND ${CMAKE_COMMAND} -E copy_directory
				"${ASWIN_EDITOR_ASSETS_DIR}"
				"${ASWIN_EDITOR_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>/assets"
			COMMENT "Copying assets to ${COMPILER_NAME}/$<CONFIG>/assets"
			VERBATIM
	)
	message(STATUS "Assets will be copied from ${ASWIN_EDITOR_ASSETS_DIR} to ${ASWIN_EDITOR_BIN_DIR}/${COMPILER_NAME}/<CONFIG>/assets at build time")
	else()
		message(WARNING "Assets directory not found: ${ASWIN_EDITOR_ASSETS_DIR}. Skipping resource copy.")
	endif()
else()
	message(STATUS "Skipping assets copy based on ASWIN_EDITOR_COPY_ASSETS option.")
endif()

# --- source files ---
include(Sources.cmake)

# --- Executable definition ---
add_executable(${PROJECT_NAME} ${PUBLIC_HEADERS} ${PRIVATE_SOURCES})

# --- Precompiled headers ---
target_precompile_headers(${PROJECT_NAME} REUSE_FROM Aswin2DEngine)

# --- Project directories ---
target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${ASWIN_EDITOR_INCLUDE_DIR}>
		$<BUILD_INTERFACE:${ASWIN_ENGINE_INCLUDE_DIR}>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${ASWIN_EDITOR_SOURCE_DIR}
		${ASWIN_ENGINE_INCLUDE_DIR}
)

# --- Project dependencies linking ---
target_link_libraries(${PROJECT_NAME} PRIVATE
	Aswin2DEngine
	imgui
)

# --- Configuration summary ---
message(STATUS "")
message(STATUS "--- Aswin2DEditor Configuration Summary ---")
message(STATUS "Project              : ${PROJECT_NAME}")
message(STATUS "Version              : ${PROJECT_VERSION}")
message(STATUS "Description          : ${PROJECT_DESCRIPTION}")
message(STATUS "Generator            : ${CMAKE_GENERATOR}")
if(CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Build Type           : Multi-config (${CMAKE_CONFIGURATION_TYPES})")
else()
	message(STATUS "Build Type           : ${CMAKE_BUILD_TYPE}")
endif()
message(STATUS "Root Directory       : ${CMAKE_SOURCE_DIR}")
message(STATUS "Build Directory      : ${CMAKE_BINARY_DIR}")
message(STATUS "Include Directory    : ${ASWIN_EDITOR_INCLUDE_DIR}")
message(STATUS "Source Directory     : ${ASWIN_EDITOR_SOURCE_DIR}")
message(STATUS "Assets Directory     : ${ASWIN_EDITOR_ASSETS_DIR}")
message(STATUS "Bin Directory        : ${ASWIN_EDITOR_BIN_DIR}")
message(STATUS "C++ Compiler         : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Compiler Path    : ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard         : ${CMAKE_CXX_STANDARD}")
message(STATUS "Copy assets          : ${ASWIN_EDITOR_COPY_ASSETS}")
if(ASWIN_EDITOR_COPY_ASSETS)
	message(STATUS "Asset Target         : ${ASWIN_EDITOR_ASSETS_DIR}/${COMPILER_NAME}/<CONFIG>/assets")
endif()
message(STATUS "")
