#include "ToolBar.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../stb/stb_image.h"

void ToolBar::setSelectedNode(std::shared_ptr<SDFTree> node)
{
    if (node != nullptr && node->getType() == SDFTree::OperationNodeType)
    {
        m_selectOpp = static_cast<OperationNode *>(node.get());
    }
    else
    {
        m_selectOpp = nullptr;
    }
}

void ToolBar::OnImGuiRender()
{
    ImGui::SetNextWindowPos(ImVec2(m_percentWidth * m_screenWidth, m_percentHeight * m_screenHeight), ImGuiCond_FirstUseEver);
    ImGui::SetWindowSize(ImVec2(200, 500));
    ImGui::Begin("Tool Bar");
    ImGui::SeparatorText("Add Shapes");

    static bool loadIcons = false;

    if (!loadIcons)
    {
        for (int i = 0; i < m_sdStrings.size(); i++)
        {
            const std::string path = std::string("./Static/") + m_sdStrings[i] + std::string(".png");
            int x = 0, y = 0, comp = 0;
            stbi_uc *bytes = stbi_load(path.c_str(), &x, &y, &comp, 4);
            if (bytes != nullptr)
            {
                auto mem = bgfx::copy(bytes, x * y * 4);
                m_sdfIcons[i] = bgfx::createTexture2D(x, y, false, 1, bgfx::TextureFormat::RGBA8, 0 | BGFX_TEXTURE_NONE, mem);
                stbi_image_free(bytes);
                std::cout << "load icons: for " << m_sdStrings[i] << std::endl;
            }
        }
        loadIcons = true;
    }

    for (int i = 0; i < m_sdStrings.size(); i++)
    {
        ImGui::PushID(i);
        if (ImGui::ImageButton(m_sdfIcons[i], ImVec2(40, 40)))
        {
            if (m_selectOpp != nullptr)
            {
                std::cout << "the button was pressed" << std::endl;
                SDFNode *newNode = new SDFNode();
                newNode->setName(m_sdStrings[i]);
                newNode->setPosition(1.0f, 1.0f, 1.0f);
                newNode->setShape(1.0f, 1.0, 1.0f);
                newNode->setNodeType(ShaderConfig::SDFType(i));
                auto newNodeShared = std::shared_ptr<SDFTree>(newNode);
                m_selectOpp->addChild(newNodeShared);
                m_shaderPtr->markForRecompile();
            }
        }
        ImGui::PopID();
    }

    ImGui::SeparatorText("Add Operations");

    for (int i = 0; i < m_opStrings.size(); i++)
    {
        if (ImGui::Button(m_opStrings[i]))
        {
            if (m_selectOpp != nullptr)
            {
                OperationNode *newOpp = new OperationNode();
                newOpp->setOperation(ShaderConfig::OperationType(i));
                newOpp->setName(m_opStrings[i]);
                auto newOppShared = std::shared_ptr<SDFTree>(newOpp);
                m_selectOpp->addChild(newOppShared);
                m_shaderPtr->markForRecompile();
            }
        }
    }

    ImGui::End();
}

ToolBar::ToolBar(/* args */)
{
}

ToolBar::~ToolBar()
{
}