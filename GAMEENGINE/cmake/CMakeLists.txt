# ==============================================================================
# Aswin2DEngine - C++ library CMake configuration
# ==============================================================================

# --- Dependencies ---
include(Dependencies.cmake)

# --- Set base directories ---
set(ASWIN_ENGINE_BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(ASWIN_ENGINE_LIB_DIR "${CMAKE_BINARY_DIR}/lib")
set(ASWIN_ENGINE_DOC_DIR "${CMAKE_BINARY_DIR}/doc")

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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ASWIN_ENGINE_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ASWIN_ENGINE_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ASWIN_ENGINE_LIB_DIR}/${COMPILER_NAME}/$<CONFIG>)

# --- Copy resources at build time ---
if(ASWIN_ENGINE_COPY_RESOURCES)
	if(EXISTS ${ASWIN_ENGINE_RESOURCE_DIR})
		add_custom_target(CopyEngineResources ALL
			COMMAND ${CMAKE_COMMAND} -E make_directory
				"${ASWIN_ENGINE_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>/resources"
			COMMAND ${CMAKE_COMMAND} -E copy_directory
				"${ASWIN_ENGINE_RESOURCE_DIR}"
				"${ASWIN_ENGINE_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>/resources"
			COMMENT "Copying resources to ${COMPILER_NAME}/$<CONFIG>/resources"
			VERBATIM
		)
		message(STATUS "Resources will be copied from ${ASWIN_ENGINE_RESOURCE_DIR} to ${ASWIN_ENGINE_BIN_DIR}/${COMPILER_NAME}/<CONFIG>/resources at build time")
	else()
		message(WARNING "Resources directory not found: ${ASWIN_ENGINE_RESOURCE_DIR}. Skipping resource copy.")
	endif()
else()
	message(STATUS "Skipping resource copy based on ASWIN_ENGINE_COPY_RESOURCES option.")
endif()

# --- source files ---
include(Sources.cmake)

# --- Library definition ---
add_library(${PROJECT_NAME} STATIC ${PUBLIC_HEADERS} ${PRIVATE_SOURCES})

# --- Precompiled headers ---
target_precompile_headers(${PROJECT_NAME} PRIVATE ${ASWIN_ENGINE_SOURCE_DIR}/pch.h)

# --- FMOD setup ---
if(WIN32)
	set(FMOD_ROOT "${ASWIN_ENGINE_EXTERNAL_LIBS}/FMOD")

	target_link_libraries(Aswin2DEngine PRIVATE
		$<$<CONFIG:Debug>:${FMOD_ROOT}/lib/fmodL_vc.lib>
		$<$<CONFIG:Debug>:${FMOD_ROOT}/lib/fmodstudioL_vc.lib>
		$<$<CONFIG:Release>:${FMOD_ROOT}/lib/fmod_vc.lib>
		$<$<CONFIG:Release>:${FMOD_ROOT}/lib/fmodstudio_vc.lib>
	)

	target_include_directories(Aswin2DEngine PRIVATE ${FMOD_ROOT}/include)

	add_custom_command(TARGET Aswin2DEngine POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<$<CONFIG:Debug>:${FMOD_ROOT}/bin/fmodL.dll>
			$<$<CONFIG:Release>:${FMOD_ROOT}/bin/fmod.dll>
			$<$<CONFIG:Debug>:${FMOD_ROOT}/bin/fmodstudioL.dll>
			$<$<CONFIG:Release>:${FMOD_ROOT}/bin/fmodstudio.dll>
			${ASWIN_ENGINE_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>/
		COMMENT "\nCopying FMOD DLLs for $<CONFIG> configuration"
	)
elseif(UNIX AND NOT APPLE)
	set(FMOD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/FMOD")
	set(FMOD_LIB_DIR "${FMOD_ROOT}/lib/linux_x86_64")
	set(FMOD_INCLUDE_DIR "${FMOD_ROOT}/include")

	target_link_libraries(Aswin2DEngine PRIVATE
		$<$<CONFIG:Debug>:${FMOD_LIB_DIR}/libfmodL.so>
		$<$<CONFIG:Debug>:${FMOD_LIB_DIR}/libfmodstudioL.so>
		$<$<CONFIG:Release>:${FMOD_LIB_DIR}/libfmod.so>
		$<$<CONFIG:Release>:${FMOD_LIB_DIR}/libfmodstudio.so>
	)

	add_custom_command(TARGET Aswin2DEngine POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${FMOD_LIB_DIR}/$<IF:$<CONFIG:Debug>,libfmodL,libfmod>.so
			${FMOD_LIB_DIR}/$<IF:$<CONFIG:Debug>,libfmodstudioL,libfmodstudio>.so
			${FMOD_LIB_DIR}/$<IF:$<CONFIG:Debug>,libfmodL,libfmod>.so.13
			${FMOD_LIB_DIR}/$<IF:$<CONFIG:Debug>,libfmodstudioL,libfmodstudio>.so.13
			${FMOD_LIB_DIR}/$<IF:$<CONFIG:Debug>,libfmodL,libfmod>.so.13.11
			${FMOD_LIB_DIR}/$<IF:$<CONFIG:Debug>,libfmodstudioL,libfmodstudio>.so.13.11
			$<TARGET_FILE_DIR:Aswin2DEngine>/
		COMMENT "\nCopying FMOD shared libraries ($<IF:$<CONFIG:Debug>,Debug,Release> versions)"
	)
endif()

# --- Project directories ---
target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${ASWIN_ENGINE_INCLUDE_DIR}>
		$<INSTALL_INTERFACE:include>
		${stb_SOURCE_DIR}
		${miniaudio_SOURCE_DIR}
		${tinyfiledialogs_SOURCE_DIR}
		${rapidjson_SOURCE_DIR}/include
		${sndfile_SOURCE_DIR}/include
		${openal_SOURCE_DIR}/include
		${ASWIN_ENGINE_EXTERNAL_LIBS}/FMOD/include
		${box2d_SOURCE_DIR}/include
		${FMOD_INCLUDE_DIR}
	PRIVATE
		${ASWIN_ENGINE_SOURCE_DIR}
)

# --- Project dependencies linking ---
target_link_libraries(${PROJECT_NAME} PUBLIC
	lua_static
	sol2::sol2
	glm::glm
	EnTT::EnTT
	box2d
	libglew_static
	glfw
	OpenAL
	sndfile
	imgui
	tinyfiledialogs
	opengl32
)

# --- OpenAL libs copying ---
message(STATUS "Setting up OpenAL runtime libraries copying")
if(WIN32)
	set(OPENAL_DLL_PATH "${CMAKE_BINARY_DIR}/_deps/openal-build/$<CONFIG>/OpenAL32.dll")
	if(NOT EXISTS "${CMAKE_BINARY_DIR}/_deps/openal-build/$<CONFIG>/OpenAL32.dll")
		set(OPENAL_DLL_PATH "${CMAKE_BINARY_DIR}/_deps/openal-build/OpenAL32.dll")
		message(STATUS "Using non-configuration specific OpenAL32.dll")
	else()
		message(STATUS "Using configuration-specific OpenAL32.dll")
	endif()

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENAL_DLL_PATH}"
			"${ASWIN_ENGINE_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>/"
		COMMENT "\nCopying OpenAL DLL to bin directory"
		VERBATIM
	)
elseif(UNIX AND NOT APPLE)
	find_file(OPENAL_SO_PATH
		NAMES "libopenal.so"
		PATHS "${CMAKE_BINARY_DIR}/_deps/openal-build"
		PATH_SUFFIXES "." "$<CONFIG>"
		NO_DEFAULT_PATH
		DOC "Path to OpenAL shared library on Linux"
	)

	if(OPENAL_SO_PATH)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${OPENAL_SO_PATH}"
				"${ASWIN_ENGINE_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>/"
			COMMENT "\nCopying OpenAL shared library to bin directory"
			VERBATIM
		)
	else()
		message(STATUS "OpenAL shared library not found for copying - application may not run correctly")
	endif()

	file(GLOB OPENAL_VERSIONED_LIBS
		"${CMAKE_BINARY_DIR}/_deps/openal-build/libopenal.so.*")

	foreach(LIB ${OPENAL_VERSIONED_LIBS})
		get_filename_component(LIB_NAME "${LIB}" NAME)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${LIB}"
				"${ASWIN_ENGINE_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>/${LIB_NAME}"
			COMMENT "\nCopying versioned OpenAL library ${LIB_NAME}"
			APPEND VERBATIM
		)
	endforeach()

	# Copy FMOD DLLs (your existing working code)
	add_custom_command(TARGET Aswin2DEngine POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${FMOD_ROOT}/bin/$<IF:$<CONFIG:Debug>,fmodL,fmod>.dll"
			"${FMOD_ROOT}/bin/$<IF:$<CONFIG:Debug>,fmodstudioL,fmodstudio>.dll"
			"${ASWIN_ENGINE_BIN_DIR}/${COMPILER_NAME}/$<CONFIG>/"
		COMMENT "Copying FMOD DLLs for $<CONFIG> configuration"
	)
endif()


# --- Samples subdirectory ---
if(ASWIN_ENGINE_BUILD_SAMPLES)
	add_subdirectory(${ASWIN_ENGINE_SAMPLES_DIR} samples)
endif()

# --- Tests subdirectory ---
if(ASWIN_ENGINE_BUILD_TESTS)
	add_subdirectory(${ASWIN_ENGINE_TEST_DIR} tests)
endif()

# --- Benchmark subdirectory ---
if(ASWIN_ENGINE_BUILD_BENCHMARKS)
	add_subdirectory(${ASWIN_ENGINE_BENCHMARK_DIR} benchmark)
endif()

# --- Doxygen documentation subdirectory ---
if(ASWIN_ENGINE_BUILD_DOCUMENTATION)
	add_subdirectory(${ASWIN_ENGINE_DOCUMENTATION_DIR} doc)
endif()

# --- Configuration summary ---
message(STATUS "")
message(STATUS "--- Aswin2DEngine Configuration Summary ---")
message(STATUS "Project                : ${PROJECT_NAME}")
message(STATUS "Version                : ${PROJECT_VERSION}")
message(STATUS "Description            : ${PROJECT_DESCRIPTION}")
message(STATUS "Generator              : ${CMAKE_GENERATOR}")
if(CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Build Type             : Multi-config (${CMAKE_CONFIGURATION_TYPES})")
else()
	message(STATUS "Build Type             : ${CMAKE_BUILD_TYPE}")
endif()
message(STATUS "Root Directory         : ${CMAKE_SOURCE_DIR}")
message(STATUS "Build Directory        : ${CMAKE_BINARY_DIR}")
message(STATUS "Include Directory      : ${ASWIN_ENGINE_INCLUDE_DIR}")
message(STATUS "Source Directory       : ${ASWIN_ENGINE_SOURCE_DIR}")
message(STATUS "Resources Directory    : ${ASWIN_ENGINE_RESOURCE_DIR}")
message(STATUS "Bin Directory          : ${ASWIN_ENGINE_BIN_DIR}")
message(STATUS "Lib Directory          : ${ASWIN_ENGINE_LIB_DIR}")
message(STATUS "Test Directory         : ${ASWIN_ENGINE_TEST_DIR}")
message(STATUS "Benchmark Directory    : ${ASWIN_ENGINE_BENCHMARK_DIR}")
message(STATUS "Sample Directory       : ${ASWIN_ENGINE_SAMPLES_DIR}")
message(STATUS "Doxygen doc Directory  : ${ASWIN_ENGINE_DOCUMENTATION_DIR}")
message(STATUS "External libs Directory: ${ASWIN_ENGINE_EXTERNAL_LIBS}")
message(STATUS "Build Shared Libs      : ${BUILD_SHARED_LIBS}")
message(STATUS "C++ Compiler           : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Compiler Path      : ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard           : ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Tests            : ${ASWIN_ENGINE_BUILD_TESTS}")
message(STATUS "Build Benchmarks       : ${ASWIN_ENGINE_BUILD_BENCHMARKS}")
message(STATUS "Build Samples          : ${ASWIN_ENGINE_BUILD_SAMPLES}")
message(STATUS "Build Documentation    : ${ASWIN_ENGINE_BUILD_DOCUMENTATION}")
message(STATUS "Copy Resources         : ${ASWIN_ENGINE_COPY_RESOURCES}")
if(ASWIN_ENGINE_COPY_RESOURCES)
	message(STATUS "Resources Target        : ${ASWIN_ENGINE_BIN_DIR}/${COMPILER_NAME}/<CONFIG>/resources")
endif()

message(STATUS "")
