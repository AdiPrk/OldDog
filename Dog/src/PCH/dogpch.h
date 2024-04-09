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

// STL
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <algorithm>
#include <math.h>
#include <unordered_map>
#include <memory>
#include <functional>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// spdlog
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

// Dog headers:
#include "Dog/Math/rect.h"