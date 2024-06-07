#ifndef NOEDEN_STDAFX_HXX
#define NOEDEN_STDAFX_HXX

#define NOMINMAX

#include <algorithm>
#include <any>
#include <array>
#include <bitset>
#include <charconv>
#include <chrono>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/chrono.h>

#include <GL/glew.h>

#include <lua5.3/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#define TOML_COMPILER_HAS_EXCEPTIONS 0
#define TOML_COMPILER_HAS_RTTI 0
#include "tomlplusplus/toml.hpp"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "stb/stb_image.h"

#include "framework/usings.hxx"
#include "framework/concepts.hxx"

#include "log/log.hxx"
#include "lang/translator.hxx"

#include "math/vec2.hxx"
#include "math/bresenham.hxx"
#include "math/rect.hxx"

#include "gfx/color.hxx"

#include "framework/event_manager.hxx"
#include "framework/events.hxx"
#include "framework/service_locator.hxx"
#include "framework/grid.hxx"

#endif
