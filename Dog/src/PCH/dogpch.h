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

// ImGui
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_node_editor.h"

// Imgui Gizmos
#include "imgui/GraphEditor.h"
#include "imgui/ImCurveEdit.h"
#include "imgui/ImGradient.h"
#include "imgui/ImGuizmo.h"
#include "imgui/ImSequencer.h"
#include "imgui/ImZoomSlider.h"

// Dog headers:
#include "Dog/Math/rect.h"
