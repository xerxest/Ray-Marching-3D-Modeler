#pragma once 
#include "imgui/imgui.h"
#include "../SDFTree.h"
#include "../ShaderBuilder.h"
#include "Panel.h"

class NodeProperties: public Panel
{
private:
    SDFTree* m_selectedNode = nullptr;
    ShaderBuilder* m_shaderPtr = nullptr;
    float m_percentWidth = 0.7f;
    float m_percentHeight = 0.7f;
public:
    NodeProperties(/* args */);
    ~NodeProperties();
    void OnImGuiRender();
   inline void setSelected(SDFTree* node) {m_selectedNode = node;}
   inline void setShader(ShaderBuilder* shader) {m_shaderPtr = shader;}
};