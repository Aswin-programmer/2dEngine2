# ==============================================================================
# Aswin 2DEngine playground - Top-level CMake configuration
# ==============================================================================

# --- Project definition ---
cmake_minimum_required(VERSION 3.20)

project(Aswin2DEnginePLayground
	VERSION 1.0.0
	DESCRIPTION "A place to test the Aswin 2DEngine."
	LANGUAGES CXX
)

# --- System Information ---
message(STATUS "")
message(STATUS "--- System Information ---")
cmake_host_system_information(RESULT TOTAL_MEMORY QUERY TOTAL_PHYSICAL_MEMORY)
cmake_host_system_information(RESULT AVAILABLE_MEMORY QUERY AVAILABLE_PHYSICAL_MEMORY)
math(EXPR MEMORY_GB "${TOTAL_MEMORY} / 1024")
message(STATUS "System Memory: ${MEMORY_GB} GB total")
message(STATUS "Platform     : ${CMAKE_HOST_SYSTEM_PROCESSOR}")

# --- Build the engine ---
add_subdirectory(GAMEENGINE)

# --- Build the editor application ---
add_subdirectory(EDITOR)

