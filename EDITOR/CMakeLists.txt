# ==============================================================================
# Aswin2DEditor - CMake configuration
# ==============================================================================

# --- Project definition ---
cmake_minimum_required(VERSION 3.20)

project(Aswin2DEditor
	VERSION 0.0.1
	DESCRIPTION "An editor application for the Aswin2DEngine"
	LANGUAGES CXX
)

# --- Build configuration ---
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "In-source builds are not allowed. Please use a separate build directory.")
endif()

# --- Project setup ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# --- Dependencies ---
#include(Dependencies.cmake)

# --- Platform specific settings ---
if(MSVC)
	set(WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
	set(BUILD_SHARED_LIBS FALSE)
endif()

# --- Build options ---
set(ASWIN_EDITOR_COPY_ASSETS ON CACHE BOOL "Copy assets to build directory" FORCE)

# --- Define project directories ---
set(ASWIN_EDITOR_INCLUDE_DIR         "${CMAKE_CURRENT_SOURCE_DIR}/include"           CACHE PATH "Path to editor header files" FORCE)
set(ASWIN_EDITOR_SOURCE_DIR          "${CMAKE_CURRENT_SOURCE_DIR}/src"               CACHE PATH "Path to editor source files" FORCE)
set(ASWIN_EDITOR_ASSETS_DIR          "${CMAKE_CURRENT_SOURCE_DIR}/assets"            CACHE PATH "Path to engine assets files" FORCE)

# --- Add core C++ library subdirectory ---
add_subdirectory(cmake)




