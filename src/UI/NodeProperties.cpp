#include "NodeProperties.h"

NodeProperties::NodeProperties(/* args */)
{
}

NodeProperties::~NodeProperties()
{
}

void NodeProperties::OnImGuiRender()
{
    setStyle();
    ImGui::SetNextWindowPos(ImVec2(m_percentWidth * m_screenWidth, m_percentHeight * m_screenHeight), ImGuiCond_FirstUseEver);
    ImGui::Begin("Node Properties");
    ImGui::SetWindowSize(ImVec2(400, 700));

    /* Root Node, Do nothing */
    if (!(m_selectedNode == nullptr || m_selectedNode->isRoot()))
    {
        // Name
        ImGui::Text("%s", m_selectedNode->getName().c_str());

        // Node Type
        if (m_selectedNode->getType() == SDFTree::SDFNodeType)
        {
            SDFNode *nodePtr = static_cast<SDFNode *>(m_selectedNode);

            ImGui::SeparatorText("Positions");
            if (ImGui::DragFloat("Pos X", &nodePtr->m_p1, 0.005f))
                m_shaderPtr->setNodeUniforms(m_selectedNode);
            if (ImGui::DragFloat("Pos Y", &nodePtr->m_p2, 0.005f))
                m_shaderPtr->setNodeUniforms(m_selectedNode);
            if (ImGui::DragFloat("Pos Z", &nodePtr->m_p3, 0.005f))
                m_shaderPtr->setNodeUniforms(m_selectedNode);
            ImGui::Dummy(ImVec2(0, 10));

            ImGui::SeparatorText("Size");
            if (ImGui::DragFloat("Size X", &nodePtr->m_s1, 0.005f))
                m_shaderPtr->setNodeUniforms(m_selectedNode);
            if (ImGui::DragFloat("Size Y", &nodePtr->m_s2, 0.005f))
                m_shaderPtr->setNodeUniforms(m_selectedNode);
            if (ImGui::DragFloat("Size Z", &nodePtr->m_s3, 0.005f))
                m_shaderPtr->setNodeUniforms(m_selectedNode);
            ImGui::Dummy(ImVec2(0, 10));

            ImGui::SeparatorText(m_sdStrings[nodePtr->getNodeType()]);
            if (ImGui::BeginListBox("##"))
            {
                for (int n = 0; n < m_sdStrings.size(); n++)
                {
                    const bool is_selected = (nodePtr->getNodeType() == n);
                    if (ImGui::Selectable(m_sdStrings[n], is_selected))
                    {
                        nodePtr->setNodeType(ShaderConfig::SDFType(n));
                        m_shaderPtr->markForRecompile();
                    }
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
        }
        else
        {
            OperationNode *oppPtr = static_cast<OperationNode *>(m_selectedNode);

            ImGui::SeparatorText(m_opStrings[oppPtr->getOperationType()]);

            ShaderConfig::OperationType currType = oppPtr->getOperationType();
            if (currType == ShaderConfig::opSmoothUnion || currType == ShaderConfig::opSmoothSubtraction || currType == ShaderConfig::opSmoothIntersection)
            {
                if(ImGui::DragFloat("Smoothness", &oppPtr->m_smoothness, 0.005f, 0.0f))
                    m_shaderPtr->setNodeUniforms(m_selectedNode);;
            }

            if (ImGui::BeginListBox("##"))
            {
                for (int n = 0; n < m_opStrings.size(); n++)
                {
                    const bool is_selected = (oppPtr->getOperationType() == n);
                    if (ImGui::Selectable(m_opStrings[n], is_selected))
                    {
                        oppPtr->setOperation(ShaderConfig::OperationType(n));
                        m_shaderPtr->markForRecompile();
                    }
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
        }
    }
    else
    {
        ImGui::TextDisabled("Root");
    }

    ImGui::End();
}