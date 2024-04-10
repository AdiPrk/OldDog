#pragma once

#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS

#define WIN32_MEAN_AND_LEAN
#define NOMINMAX
#include <Windows.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/GL.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Common PCH
#include <common_pch.h>

// STL
#include <chrono>
#include <thread>

// spdlog
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

// Dog headers:
#include "Dog/Math/rect.h"