#include "Help.h"

void Help::OnImGuiRender()
{
    ImGui::Begin("Help");
    ImGui::SetWindowSize(ImVec2(400, 250));
    ImGui::SeparatorText("Controls");
    ImGui::Text("Right Mouse + Cursor - Look Around");
    ImGui::Text("W - Forward");
    ImGui::Text("A - Left");
    ImGui::Text("D - Right");
    ImGui::Text("S - Backward");
    ImGui::Text("(Note) Operation Node must be Selected to added SDF Shape");
    ImGui::End();
}