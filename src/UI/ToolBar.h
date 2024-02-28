#pragma once 
#include <string>
#include <vector>
#include "imgui/imgui.h"

class ToolBar
{
private:
    /* data */
    const std::vector<const std::string> m_sdButtonTitleName = 
    {
        "Sphere",
        "Box",
        "RoundBox",
        "Box",
        "Torus",
        "Cylinder",
        "Cone",
        "Plane",
        "HexPrism",
        "TriPrism"
    };

public:
    ToolBar(/* args */);
    ~ToolBar();
    void OnImGuiRender();
};
