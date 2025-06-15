# ==============================================================================
# Aswin2DEngine- C++ Library CMake Dependencies
# ==============================================================================

message(STATUS "")
message(STATUS "--- Fetching dependencies ---")

# --- Set FetchContent properties ---
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# --- sol2 ---
set(SOL2_CI                        OFF  CACHE BOOL  "Whether or not we are in continguous integration mode"      FORCE)
set(SOL2_SYSTEM_INCLUDE            OFF  CACHE BOOL  "Whether or not sol2 should be considered a system include." FORCE)
set(SOL2_TESTS                     OFF  CACHE BOOL  "Enable build of tests"                                      FORCE)
set(SOL2_EXAMPLES                  OFF  CACHE BOOL  "Enable build of examples"                                   FORCE)
set(SOL2_INTEROP_EXAMPLES          OFF  CACHE BOOL  "Enable build of interop examples"                           FORCE)
set(SOL2_DYNAMIC_LOADING_EXAMPLES  OFF  CACHE BOOL  "Enable build of interop examples"                           FORCE)
set(SOL2_SINGLE                    OFF  CACHE BOOL  "Enable generation and build of single header files"         FORCE)
set(SOL2_DOCS                      OFF  CACHE BOOL  "Enable build of documentation"                              FORCE)
set(SOL2_ENABLE_INSTALL            OFF  CACHE BOOL  "Enable installation of Sol2"                                FORCE)

FetchContent_Declare(
	sol2
	GIT_REPOSITORY https://github.com/ThePhD/sol2.git
	GIT_TAG        v3.3.1
	GIT_SHALLOW    TRUE
)

# --- glm ---
set(GLM_ENABLE_EXPERIMENTAL ON CACHE BOOL "" FORCE)

set(GLM_BUILD_LIBRARY           OFF  CACHE BOOL  "Build dynamic/static library"                                FORCE)
set(GLM_BUILD_TESTS             OFF  CACHE BOOL  "Build the test programs"                                     FORCE)
set(GLM_BUILD_INSTALL           OFF  CACHE BOOL  "Generate the install target"                                 FORCE)
set(GLM_ENABLE_CXX_98           OFF  CACHE BOOL  "Enable C++ 98"                                               FORCE)
set(GLM_ENABLE_CXX_11           OFF  CACHE BOOL  "Enable C++ 11"                                               FORCE)
set(GLM_ENABLE_CXX_14           OFF  CACHE BOOL  "Enable C++ 14"                                               FORCE)
set(GLM_ENABLE_CXX_17           OFF  CACHE BOOL  "Enable C++ 17"                                               FORCE)
set(GLM_ENABLE_CXX_20           ON   CACHE BOOL  "Enable C++ 20"                                               FORCE)
set(GLM_ENABLE_LANG_EXTENSIONS  OFF  CACHE BOOL  "Enable language extensions"                                  FORCE)
set(GLM_DISABLE_AUTO_DETECTION  OFF  CACHE BOOL  "Disable platform, compiler, arch and C++ language detection" FORCE)
set(GLM_ENABLE_FAST_MATH        ON   CACHE BOOL  "Enable fast math optimizations"                              FORCE)
set(GLM_ENABLE_SIMD_SSE2        OFF  CACHE BOOL  "Enable SSE2 optimizations"                                   FORCE)
set(GLM_ENABLE_SIMD_SSE3        OFF  CACHE BOOL  "Enable SSE3 optimizations"                                   FORCE)
set(GLM_ENABLE_SIMD_SSSE3       OFF  CACHE BOOL  "Enable SSSE3 optimizations"                                  FORCE)
set(GLM_ENABLE_SIMD_SSE4_1      OFF  CACHE BOOL  "Enable SSE 4.1 optimizations"                                FORCE)
set(GLM_ENABLE_SIMD_SSE4_2      OFF  CACHE BOOL  "Enable SSE 4.2 optimizations"                                FORCE)
set(GLM_ENABLE_SIMD_AVX         OFF  CACHE BOOL  "Enable AVX optimizations"                                    FORCE)
set(GLM_ENABLE_SIMD_AVX2        OFF  CACHE BOOL  "Enable AVX2 optimizations"                                   FORCE)
set(GLM_ENABLE_SIMD_NEON        OFF  CACHE BOOL  "Enable ARM NEON optimizations"                               FORCE)
set(GLM_FORCE_PURE              OFF  CACHE BOOL  "Force 'pure' instructions"                                   FORCE)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG        1.0.1
	GIT_SHALLOW    TRUE
)

# --- EnTT ---
set(ENTT_BUILD_EXAMPLE    OFF  CACHE BOOL  "Build examples."                                        FORCE)
set(ENTT_USE_LIBCPP       OFF  CACHE BOOL  "Use libc++ by adding -stdlib=libc++ flag if available." FORCE)
set(ENTT_USE_SANITIZER    OFF  CACHE BOOL  "Enable sanitizers."                                     FORCE)
set(ENTT_USE_CLANG_TIDY   OFF  CACHE BOOL  "Enable static analysis with clang-tidy"                 FORCE)
set(ENTT_INCLUDE_HEADERS  OFF  CACHE BOOL  "Add all EnTT headers to the EnTT target."               FORCE)
set(ENTT_INCLUDE_TOOLS    OFF  CACHE BOOL  "Add EnTT tools files to the EnTT target."               FORCE)
set(ENTT_INCLUDE_NATVIS   OFF  CACHE BOOL  "Add EnTT natvis files to the EnTT target."              FORCE)
set(ENTT_INSTALL          OFF  CACHE BOOL  "Install EnTT and all related files."                    FORCE)
set(ENTT_BUILD_TESTBED    OFF  CACHE BOOL  "Enable building testbed."                               FORCE)
set(ENTT_BUILD_TESTING    OFF  CACHE BOOL  "Enable building tests."                                 FORCE)
set(ENTT_BUILD_DOCS       OFF  CACHE BOOL  "Enable building with documentation."                    FORCE)

FetchContent_Declare(
	EnTT
	GIT_REPOSITORY https://github.com/skypjack/entt.git
	GIT_TAG        v3.15.0
	GIT_SHALLOW    TRUE
)

# --- box2d ---
FetchContent_Declare(
	box2d
	GIT_REPOSITORY https://github.com/erincatto/box2d.git
	GIT_TAG        v3.1.1
	GIT_SHALLOW    TRUE
)

# --- glfw ---
set(GLFW_BUILD_EXAMPLES  OFF  CACHE BOOL  "Build the GLFW example programs" FORCE)
set(GLFW_BUILD_TESTS     OFF  CACHE BOOL  "Build the GLFW test programs"    FORCE)
set(GLFW_BUILD_DOCS      OFF  CACHE BOOL  "Build the GLFW documentation"    FORCE)
set(GLFW_INSTALL         OFF  CACHE BOOL  "Generate installation target"    FORCE)

FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG        3.4
	GIT_SHALLOW    TRUE
)

# --- stb ---
set(STB_IMAGE_IMPLEMENTATION      ON CACHE BOOL  "" FORCE)
set(STB_RECT_PACK_IMPLEMENTATION  ON CACHE BOOL  "" FORCE)
set(STB_TRUETYPE_IMPLEMENTATION   ON CACHE BOOL  "" FORCE)

FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG        master
	GIT_SHALLOW    TRUE
)

# --- miniaudio ---
set(MINIAUDIO_IMPLEMENTATION  ON CACHE BOOL  "" FORCE)

FetchContent_Declare(
	miniaudio
	GIT_REPOSITORY https://github.com/mackron/miniaudio.git
	GIT_TAG        0.11.21
	GIT_SHALLOW    TRUE
)

# --- Make all dependencies available ---
FetchContent_MakeAvailable(sol2 glm EnTT box2d glfw stb miniaudio)

# --- OLD POLICTY ---
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

# --- lua ---
set(LUA_SUPPORT_DL         OFF  CACHE BOOL  "Support dynamic loading of compiled modules" FORCE)
set(LUA_BUILD_AS_CXX       OFF  CACHE BOOL  "Build lua as C++"                            FORCE)
set(LUA_ENABLE_SHARED      OFF  CACHE BOOL  "Build dynamic liblua"                        FORCE)
set(LUA_ENABLE_TESTING     OFF  CACHE BOOL  "Build and run tests"                         FORCE)
set(LUA_BUILD_COMPILER     OFF  CACHE BOOL  "Build the luac compiler executable"          FORCE)
set(LUA_BUILD_INTERPRETER  OFF  CACHE BOOL  "Build the lua interpreter executable"        FORCE)

FetchContent_Declare(
	lua
	GIT_REPOSITORY https://github.com/walterschell/Lua.git
	GIT_TAG        v5.4.5
	GIT_SHALLOW    TRUE
)

# --- glew ---
set(glew-cmake_BUILD_SHARED           OFF  CACHE BOOL  "Build the shared glew library"                                                FORCE)
set(glew-cmake_BUILD_STATIC           ON   CACHE BOOL  "Build the static glew library"                                                FORCE)
set(USE_GLU                           OFF  CACHE BOOL  "Use GLU"                                                                      FORCE)
set(GLEW_OSMESA                       OFF  CACHE BOOL  "Off-screen Mesa mode"                                                         FORCE)
set(PKG_CONFIG_REPRESENTATIVE_TARGET  OFF  CACHE BOOL  "Generate pc file for specified target as glew. libglew_static|libglew_shared" FORCE)
set(ONLY_LIBS                         OFF  CACHE BOOL  "Do not build executables"                                                     FORCE)

FetchContent_Declare(
	glew
	GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
	GIT_TAG glew-cmake-2.2.0
	GIT_SHALLOW    TRUE
)

# --- openal ---
set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG Release CACHE STRING "" FORCE)
set(ALSOFT_DLOPEN                  ON   CACHE BOOL  "Check for the dlopen API for loading optional libs"         FORCE)
set(ALSOFT_WERROR                  OFF  CACHE BOOL  "Treat compile warnings as errors"                           FORCE)
set(ALSOFT_UTILS                   OFF  CACHE BOOL  "Build utility programs"                                     FORCE)
set(ALSOFT_NO_CONFIG_UTIL          ON   CACHE BOOL  "Disable building the alsoft-config utility"                 FORCE)
set(ALSOFT_EXAMPLES                OFF  CACHE BOOL  "Build example programs"                                     FORCE)
set(ALSOFT_TESTS                   OFF  CACHE BOOL  "Build test programs"                                        FORCE)
set(ALSOFT_INSTALL                 OFF  CACHE BOOL  "Install main library"                                       FORCE)
set(ALSOFT_INSTALL_CONFIG          OFF  CACHE BOOL  "Install alsoft.conf sample configuration file"              FORCE)
set(ALSOFT_INSTALL_HRTF_DATA       OFF  CACHE BOOL  "Install HRTF data files"                                    FORCE)
set(ALSOFT_INSTALL_AMBDEC_PRESETS  OFF  CACHE BOOL  "Install AmbDec preset files"                                FORCE)
set(ALSOFT_INSTALL_EXAMPLES        OFF  CACHE BOOL  "Install example programs (alplay, alstream, ...)"           FORCE)
set(ALSOFT_INSTALL_UTILS           OFF  CACHE BOOL  "Install utility programs (openal-info, alsoft-config, ...)" FORCE)
set(ALSOFT_UPDATE_BUILD_VERSION    OFF  CACHE BOOL  "Update git build version info"                              FORCE)
set(ALSOFT_SEARCH_INSTALL_DATADIR  OFF  CACHE BOOL  "Search the installation data directory"                     FORCE)

FetchContent_Declare(
	openal
	GIT_REPOSITORY https://github.com/kcat/openal-soft.git
	GIT_TAG        1.24.3
	GIT_SHALLOW    TRUE
)

# msvc hack to force building as release .lb and .dll
set(CMAKE_BUILD_TYPE_ORIG ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE Release)
FetchContent_MakeAvailable(openal)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_ORIG})

# --- libsndfile ---
if(MSVC AND WIN32)
	set(HAVE_WINSOCK2_H    ON   CACHE BOOL "Force use of winsock2.h for libsndfile"         FORCE)
	set(HAVE_SYS_TIME_H    OFF  CACHE BOOL "Force disable use of sys/time.h for libsndfile" FORCE)
	set(HAVE_GETTIMEOFDAY  OFF  CACHE BOOL "Force disable gettimeofday for libsndfile"      FORCE)
endif()

set(BUILD_TESTING        OFF CACHE BOOL "" FORCE)

set(ENABLE_EXTERNAL_LIBS      OFF  CACHE BOOL  "Enable FLAC, Vorbis, and Opus codecs"     FORCE)
set(ENABLE_MPEG               OFF  CACHE BOOL  "Enable MPEG codecs"                       FORCE)
set(ENABLE_EXPERIMENTAL       OFF  CACHE BOOL  "Enable experimental code"                 FORCE)
set(BUILD_REGTEST             OFF  CACHE BOOL  "Build regtest"                            FORCE)
set(BUILD_SHARED_LIBS         OFF  CACHE BOOL  "Build shared libraries"                   FORCE)
set(BUILD_PROGRAMS            OFF  CACHE BOOL  "Build programs"                           FORCE)
set(BUILD_EXAMPLES            OFF  CACHE BOOL  "Build examples"                           FORCE)
set(ENABLE_CPACK              OFF  CACHE BOOL  "Enable CPack support"                     FORCE)
set(ENABLE_BOW_DOCS           OFF  CACHE BOOL  "Enable black-on-white html docs"          FORCE)
set(ENABLE_PACKAGE_CONFIG     OFF  CACHE BOOL  "Generate and install package config file" FORCE)
set(INSTALL_PKGCONFIG_MODULE  OFF  CACHE BOOL  "Generate and install pkg-config module"   FORCE)

FetchContent_Declare(
	sndfile
	GIT_REPOSITORY https://github.com/libsndfile/libsndfile.git
	GIT_TAG        1.2.2
	GIT_SHALLOW    TRUE
)

# --- rapidjson ---
set(RAPIDJSON_BUILD_DOC                   OFF  CACHE BOOL  "Build rapidjson documentation."                                       FORCE)
set(RAPIDJSON_BUILD_EXAMPLES              OFF  CACHE BOOL  "Build rapidjson examples."                                            FORCE)
set(RAPIDJSON_BUILD_TESTS                 OFF  CACHE BOOL  "Build rapidjson perftests and unittests."                             FORCE)
set(RAPIDJSON_BUILD_THIRDPARTY_GTEST      OFF  CACHE BOOL  "Use gtest installation in `thirdparty/gtest` by default if available" FORCE)
set(RAPIDJSON_BUILD_CXX11                 OFF  CACHE BOOL  "Build rapidjson with C++11"                                           FORCE)
set(RAPIDJSON_BUILD_CXX17                 OFF  CACHE BOOL  "Build rapidjson with C++17"                                           FORCE)
set(RAPIDJSON_BUILD_CXX20                 ON   CACHE BOOL  "Build rapidjson with C++20"                                           FORCE)
set(RAPIDJSON_BUILD_ASAN                  OFF  CACHE BOOL  "Build rapidjson with address sanitizer (gcc/clang)"                   FORCE)
set(RAPIDJSON_BUILD_UBSAN                 OFF  CACHE BOOL  "Build rapidjson with undefined behavior sanitizer (gcc/clang)"        FORCE)
set(RAPIDJSON_ENABLE_INSTRUMENTATION_OPT  OFF  CACHE BOOL  "Build rapidjson with -march or -mcpu options"                         FORCE)

FetchContent_Declare(
	rapidjson
	GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
	GIT_TAG        v1.1.0
	GIT_SHALLOW    TRUE
)

# --- Make all dependencies available ---
FetchContent_MakeAvailable(lua glew sndfile rapidjson)


# --- tinyfiledialogs ---
FetchContent_Declare(
	tinyfiledialogs_lib
	GIT_REPOSITORY https://github.com/native-toolkit/libtinyfiledialogs.git
	GIT_TAG        master
	GIT_SHALLOW    TRUE
)

# --- imgui ---
set(IMGUI_BUILD_EXAMPLES  OFF CACHE BOOL  "" FORCE)

FetchContent_Declare(
	imgui_lib
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG        docking
	GIT_SHALLOW    TRUE
)


FetchContent_MakeAvailable(tinyfiledialogs_lib imgui_lib)


# --- tinyfiledialogs target---
add_library(tinyfiledialogs STATIC
	"${tinyfiledialogs_lib_SOURCE_DIR}/tinyfiledialogs.c"
)

target_include_directories(tinyfiledialogs PUBLIC
	"${tinyfiledialogs_lib_SOURCE_DIR}"
)

# --- imgui target ---
add_library(imgui STATIC
	${imgui_lib_SOURCE_DIR}/imgui.cpp
	${imgui_lib_SOURCE_DIR}/imgui_demo.cpp
	${imgui_lib_SOURCE_DIR}/imgui_draw.cpp
	${imgui_lib_SOURCE_DIR}/imgui_tables.cpp
	${imgui_lib_SOURCE_DIR}/imgui_widgets.cpp
	${imgui_lib_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	${imgui_lib_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)


target_include_directories(imgui PUBLIC
	${imgui_lib_SOURCE_DIR}
	${imgui_lib_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw ${OPENGL_LIBRARIES})
