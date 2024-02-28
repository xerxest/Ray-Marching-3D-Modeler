#include "TreeView.h"

TreeView::TreeView(SDFTree* root)
{
    m_rootScene = root;
}

TreeView::~TreeView()
{
}

void TreeView::displayChild(SDFTree* node)
{
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (m_selectedNode == node)
    {
        node_flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool is_open = ImGui::TreeNodeEx((void*)(intptr_t)(int)node->m_bufferIndex, node_flags, "%s", node->getName().c_str());

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
         m_newSelectedNode = node;
    }

    if(is_open)
    {
        int childIndex = 0;
        int len = node->childCount();
        while(childIndex < len)
        {
            displayChild(node->child(childIndex));
            childIndex++;
        }
        ImGui::TreePop();
    }
}

void TreeView::OnImGuiRender()
{
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    static bool align_label_with_current_x_position = false;
    static bool test_drag_and_drop = false;
    if (align_label_with_current_x_position)
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

    m_newSelectedNode =  nullptr;
    
    displayChild(m_rootScene);

    // Update Selected Node
    if (m_newSelectedNode != nullptr)
    {
        // Update selection state
        m_selectedNode = m_newSelectedNode;
    }

    if (align_label_with_current_x_position)
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
}