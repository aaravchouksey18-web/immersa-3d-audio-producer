
#ifndef IMGUI_INPUT_EXTENDED_H
#define IMGUI_INPUT_EXTENDED_H

#include "imgui.h"

//input float, but with limits set
bool ImGui_Extended_InputFloat(const char* label, float* v, 
                float step = 0.0f, float step_fast = 0.0f, 
                const char* format = "%.3f", 
                float min_limit = -1E6, float max_limit = 1E6,
                ImGuiInputTextFlags flags = 0
                );
#endif