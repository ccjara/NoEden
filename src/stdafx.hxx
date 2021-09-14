#ifndef JARALYN_STDAFX_HXX
#define JARALYN_STDAFX_HXX

#define NOMINMAX

#include <algorithm>
#include <charconv>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <unordered_map>

#define FMT_HEADER_ONLY
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
// #include <fmt/format.h> -- included by spdlog
#include <fmt/chrono.h>

#include <GL/glew.h>

#include "lua.hpp"
#include <LuaBridge/LuaBridge.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <entt/entt.hxx>

using u32 = uint32_t;
using u64 = uint64_t;
using i32 = int32_t;
using i64 = int64_t;

namespace fs = std::filesystem;

#include "log/log.hxx"

#include "math/vec2.hxx"
#include "math/bresenham.hxx"
#include "math/rect.hxx"

#include "gfx/color.hxx"

#endif
