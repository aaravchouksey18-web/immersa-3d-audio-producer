#include "imgui_input_extended.h"

bool ImGui_Extended_InputFloat(const char* label, float* v, 
                float step, float step_fast, 
                const char* format, 
                float min_limit, float max_limit,
                ImGuiInputTextFlags flags
                ){

    bool status = ImGui::InputFloat(label, v, step, step_fast, format, flags);

    if(*v < min_limit){*v = min_limit;}
    if(*v > max_limit){*v = max_limit;}

    return status;
}
