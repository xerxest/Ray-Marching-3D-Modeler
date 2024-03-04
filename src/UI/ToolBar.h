#pragma once 
#include "imgui/imgui.h"
#include "../SDFTree.h"
#include "../ShaderBuilder.h"
#include "Panel.h"
#include <string>
#include <vector>

class ToolBar: public Panel
{
private:
    float m_percentWidth = 0.1f;
    float m_percentHeight = 0.5f;
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
    OperationNode* m_selectOpp = nullptr;
    ShaderBuilder* m_shaderPtr = nullptr;

public:
    ToolBar(/* args */);
    ~ToolBar();
    void OnImGuiRender();
    void setSelectedNode(std::shared_ptr<SDFTree> node);
    bgfx::TextureHandle m_sdfIcons[7];
    inline void setShader(ShaderBuilder* shader) {m_shaderPtr = shader;}
};
