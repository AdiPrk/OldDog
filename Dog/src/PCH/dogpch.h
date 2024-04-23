#pragma once

#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS

#define WIN32_MEAN_AND_LEAN
#define NOMINMAX
#include <Windows.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
// #include <GLFW/glfw3native.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

// STL
#include <chrono>
#include <thread>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <math.h>
#include <typeindex>
#include <mutex>
#include <filesystem>
#include <random>

// spdlog
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

// ImGui
#ifndef DOG_SHIP

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_node_editor.h"

// ImGui Gizmos
#include "GraphEditor.h"
#include "ImCurveEdit.h"
#include "ImGradient.h"
#include "ImGuizmo.h"
#include "ImSequencer.h"
#include "ImZoomSlider.h"

// ImGui Text Editor
#include "ImGuiColorTextEdit/TextEditor.h"

#endif

// EnTT
#include "entt/entt.hpp"

// Yaml
#define YAML_CPP_STATIC_DEFINE
#include "yaml-cpp/yaml.h"

// Dog headers:
#include "Dog/Math/rect.h"
#include "Dog/Assets/UUID/uuid.h"
#include "Dog/Logger/logger.h"
#include "Dog/Events/event.h"
#include "Dog/Assets/asset.h"
#include "Dog/Assets/assets.h"
