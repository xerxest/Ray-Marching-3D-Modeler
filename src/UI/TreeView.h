#pragma once
#include "imgui/imgui.h"
#include "../SDFTree.h"
#include "Panel.h"

// Basically a wrapper around a SDFTree node for GUI purposes
// struct DisplayNode
// {
//     SDFTree* nodePtr;
//     bool isSelected;
//     std::vector<DisplayNode*> childrenPtr;
// };

class TreeView: public Panel
{
private:
    SDFTree* m_rootScene;
    void displayChild(SDFTree* node);
    int selection_mask = (1 << 2);
    SDFTree* m_selectedNode;
    SDFTree* m_newSelectedNode;
    float m_percentWidth = 0.8f;
    float m_percentHeight = 0.0f;
public:
    TreeView(SDFTree* root);
    ~TreeView();
    void OnImGuiRender();
    SDFTree* getSelectedNode();
};
