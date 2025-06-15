#pragma once

#ifdef _WIN32
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL

/* C++ STL */
#include <algorithm>
#include <any>
#include <array>
#include <cassert>
#include <chrono>
#include <cinttypes>
#include <csignal>
#include <ctime>
#include <cstdint>
#include <iostream>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <list>
#include <map>
#include <memory>
#include <random>
#include <ranges>
#include <set>
#include <source_location>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <time.h>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

/* External libs */
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <box2d/box2d.h>
#include <entt/entt.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <miniaudio.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/error/en.h>
#include <sndfile.h>
#include <sol/sol.hpp>
#include <tinyfiledialogs.h>
