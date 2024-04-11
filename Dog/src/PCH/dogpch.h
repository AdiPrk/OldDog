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
#include <GLFW/glfw3native.h>

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
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_node_editor.h"

// Imgui Gizmos
#include "GraphEditor.h"
#include "ImCurveEdit.h"
#include "ImGradient.h"
#include "ImGuizmo.h"
#include "ImSequencer.h"
#include "ImZoomSlider.h"

// Dog headers:
#include "Dog/Math/rect.h"
