#pragma once
#include "imgui/imgui.h"
#include "../SDFTree.h"
#include "Panel.h"

class TreeView: public Panel
{
private:
    std::shared_ptr<SDFTree> m_rootScene;
    void displayChild(std::shared_ptr<SDFTree> node);
    int selection_mask = (1 << 2);
    std::shared_ptr<SDFTree> m_selectedNode;
    std::shared_ptr<SDFTree> m_newSelectedNode;
    float m_percentWidth = 0.8f;
    float m_percentHeight = 0.0f;
public:
    TreeView(std::shared_ptr<SDFTree> root);
    ~TreeView();
    void OnImGuiRender();
    std::shared_ptr<SDFTree> getSelectedNode();
};
