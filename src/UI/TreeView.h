#pragma once
#include "imgui/imgui.h"
#include "../SDFTree.h"

// Basically a wrapper around a SDFTree node for GUI purposes
// struct DisplayNode
// {
//     SDFTree* nodePtr;
//     bool isSelected;
//     std::vector<DisplayNode*> childrenPtr;
// };

class TreeView
{
private:
    SDFTree* m_rootScene;
    void displayChild(SDFTree* node);
    int selection_mask = (1 << 2);
    SDFTree* m_selectedNode;
    SDFTree* m_newSelectedNode;
public:
    TreeView(SDFTree* root);
    ~TreeView();
    void OnImGuiRender();
};
