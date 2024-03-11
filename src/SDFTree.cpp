#include "SDFTree.h"

std::string SDFTree::compileToGlsl() const 
{
    if(childCount() > 0)
    {
        return child(0)->compileToGlsl();
    }
    
    return "0";
}


void SDFTree::addChild(std::shared_ptr<SDFTree> node)
{   
    node->m_parent = this;
    // auto newNode = std::make_shared<SDFTree>(node);
    m_children.push_back(node);
}

void SDFTree::removeChild(std::shared_ptr<SDFTree> node)
{
    auto it = std::find_if(m_children.begin(), m_children.end(), 
                           [&node](const std::shared_ptr<SDFTree>& child) {
                               return child == node;
                           });
    if (it != m_children.end()) {
        m_children.erase(it);
    }
}


std::string SDFNode::compileToGlsl() const
{   
    switch (m_nodeType)
    {
    case ShaderConfig::sdSphere:
        return ShaderConfig::sdSphere_glsl(m_bufferIndex);
        break;
    
    case ShaderConfig::udBox:
        return ShaderConfig::udBox_glsl(m_bufferIndex);
        break;
    
    case ShaderConfig::udRoundBox:
        return ShaderConfig::udRoundBox_glsl(m_bufferIndex);
        break;
    
    case ShaderConfig::sdTorus:
        return ShaderConfig::sdTorus_glsl(m_bufferIndex);
        break;
    
    case ShaderConfig::sdCone:
        return ShaderConfig::sdCone_glsl(m_bufferIndex);
        break;

    case ShaderConfig::sdHexPrism:
        return ShaderConfig::sdHexPrism_glsl(m_bufferIndex);
        break;

    case ShaderConfig::sdTriPrism:
        return ShaderConfig::sdTriPrism_glsl(m_bufferIndex);
        break;
    
    default:
        break;
    }

    return "";

}


std::string OperationNode::wrapAllChildrenWithOpp(std::string (*func)(std::string,std::string)) const
{
    std::string glsl = "0";
		for (size_t i = 0; i < childCount(); ++i)
		{
			std::string childGlsl = child(i)->compileToGlsl();
            if(childGlsl == "") childGlsl = "0";
			glsl = i == 0 ? childGlsl : func(glsl,childGlsl);
		}
    return glsl;
}

std::string OperationNode::wrapAllChildrenWithSmoothOpp(std::string (*func)(std::string,std::string,int)) const
{
    std::string glsl = "0";
		for (size_t i = 0; i < childCount(); ++i)
		{
			std::string childGlsl = child(i)->compileToGlsl();
            if(childGlsl == "") childGlsl = "0";
			glsl = i == 0 ? childGlsl : func(glsl,childGlsl,m_smoothBufferIndex);
		}
    return glsl;
}

std::string OperationNode::compileToGlsl() const
{   
    switch (m_opp)
    {
    case ShaderConfig::opUnion:
        return wrapAllChildrenWithOpp(ShaderConfig::opUnion_glsl);
        break;
    case ShaderConfig::opSubtract:
        return wrapAllChildrenWithOpp(ShaderConfig::opSubtract_glsl);
        break;
    case ShaderConfig::opIntersect:
        return wrapAllChildrenWithOpp(ShaderConfig::opIntersect_glsl);
        break;
    case ShaderConfig::opSmoothUnion:
        return wrapAllChildrenWithSmoothOpp(ShaderConfig::opSmoothUnion_glsl);
        break;
    case ShaderConfig::opSmoothSubtraction:
        return wrapAllChildrenWithSmoothOpp(ShaderConfig::opSmoothSubtraction_glsl);
        break;
    case ShaderConfig::opSmoothIntersection:
        return wrapAllChildrenWithSmoothOpp(ShaderConfig::opSmoothIntersection_glsl);
        break;
    default:
        break;
    }
    return "";
}


SDFTree::SDFTree() noexcept : m_parent(nullptr), m_bufferIndex(0), m_numChildren(0) {}

SDFTree::~SDFTree() {
    // Add any necessary cleanup code here
}