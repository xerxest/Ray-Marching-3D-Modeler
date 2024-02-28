#include "ToolBar.h"

ToolBar::ToolBar(/* args */)
{
}

ToolBar::~ToolBar()
{
}


void ToolBar::OnImGuiRender()
{

    ImGui::Begin("Add Shape");

    for(auto titleStr: m_sdButtonTitleName)
    {
        ImGui::Button(titleStr.c_str());
    }

    ImGui::End();

}