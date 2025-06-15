# ==============================================================================
# Aswin2DEngine - CMake configuration
# ==============================================================================

# --- Project definition ---
cmake_minimum_required(VERSION 3.20)

project(Aswin2DEngine
	VERSION 0.0.1
	DESCRIPTION "Learning and Making a Game Engine with the help of youtube, google etc."
	LANGUAGES
		C
		CXX
)

# --- Build configuration ---
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "In-source builds are not allowed. Please use a separate build directory.")
endif()

# --- Testing setup (for engine unit test) ---
enable_testing()

# --- Project setup ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# --- Platform specific settings ---
if(MSVC)
	set(WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
	set(BUILD_SHARED_LIBS FALSE)
endif()

# --- Build options ---
set(ASWIN_ENGINE_BUILD_TESTS          OFF CACHE BOOL "Build tests"                               FORCE)
set(ASWIN_ENGINE_BUILD_BENCHMARKS     OFF CACHE BOOL "Build benchmarks"                          FORCE)
set(ASWIN_ENGINE_COPY_RESOURCES       ON  CACHE BOOL "Copy resources to build directory"         FORCE)
set(ASWIN_ENGINE_BUILD_SAMPLES        OFF CACHE BOOL "Build Aswin2DEngine samples"               FORCE)
set(ASWIN_ENGINE_BUILD_DOCUMENTATION  OFF CACHE BOOL "Build Aswin2DEngine doxygen documentation" FORCE)

# --- Project directories ---
set(ASWIN_ENGINE_INCLUDE_DIR         "${CMAKE_CURRENT_SOURCE_DIR}/include"           CACHE PATH "Path to engine header files"    FORCE)
set(ASWIN_ENGINE_SOURCE_DIR          "${CMAKE_CURRENT_SOURCE_DIR}/src"               CACHE PATH "Path to engine source files"    FORCE)
set(ASWIN_ENGINE_TEST_DIR            "${CMAKE_CURRENT_SOURCE_DIR}/test"              CACHE PATH "Path to engine test files"      FORCE)
set(ASWIN_ENGINE_BENCHMARK_DIR       "${CMAKE_CURRENT_SOURCE_DIR}/benchmark"         CACHE PATH "Path to engine benchmark files" FORCE)
set(ASWIN_ENGINE_SAMPLES_DIR         "${CMAKE_CURRENT_SOURCE_DIR}/samples"           CACHE PATH "Path to engine sample projects" FORCE)
set(ASWIN_ENGINE_DOCUMENTATION_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/doc"               CACHE PATH "Path to engine documentation"   FORCE)
set(ASWIN_ENGINE_RESOURCE_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/resources"         CACHE PATH "Path to engine resource files"  FORCE)
set(ASWIN_ENGINE_EXTERNAL_LIBS       "${CMAKE_CURRENT_SOURCE_DIR}/libs"              CACHE PATH "Path to external libraries"     FORCE)

# --- Compiler settings ---
include(${CMAKE_SOURCE_DIR}/cmake/CompilersSettings.cmake)

# --- Add core C++ library subdirectory ---
add_subdirectory(cmake)
