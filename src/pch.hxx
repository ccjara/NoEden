#ifndef NOEDEN_PCH_HXX
#define NOEDEN_PCH_HXX

#ifndef NOEDEN_RELEASE
#define NOEDEN_XRAY
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#endif

#define NOMINMAX

#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <charconv>
#include <chrono>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <experimental/scope>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <iterator>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <shared_mutex>
#include <source_location>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <GL/glew.h>

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <edenjson.hxx>

#define TOML_COMPILER_HAS_EXCEPTIONS 0
#define TOML_COMPILER_HAS_RTTI 0
#include <toml++/toml.hpp>

#ifdef NOEDEN_XRAY
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#endif

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define SPDLOG_FMT_EXTERNAL 1
#define SPDLOG_COMPILED_LIB 1

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "stb/stb_image.h"
#include "stb/stb_perlin.h"

#include "framework/macros.hxx"
#include "framework/usings.hxx"
#include "framework/concepts.hxx"
#include "framework/event.hxx"
#include "framework/thread_pool.hxx"

#include "instrumentation/profiler.hxx"

#include "input/input_state.hxx"
#include "input/input_event.hxx"

#include "log/log.hxx"
#include "lang/translator.hxx"

#include "math/bresenham.hxx"
#include "math/rect.hxx"

#include "gfx/color.hxx"

#include "framework/event_manager.hxx"
#include "framework/service_locator.hxx"
#include "framework/grid.hxx"

#include "world/world_pos.hxx"
#include "world/chunk_pos.hxx"

#endif
