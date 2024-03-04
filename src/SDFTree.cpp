#include "SDFTree.h"

std::string SDFTree::compileToGlsl() const
{
    if(isRoot())
    {
        return m_children[0]->compileToGlsl();
    }

    return "";
}


void SDFTree::addChild(SDFTree *node)
{   
    node->m_parent = this;
    m_children.push_back(node);
}


std::string SDFNode::compileToGlsl() const
{   
    // TODO use uniform Values
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
    std::string glsl = "";
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
    std::string glsl = "";
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


SDFTree::SDFTree() : m_parent(nullptr), m_bufferIndex(0), m_numChildren(0) {}

SDFTree::~SDFTree() {
    // Add any necessary cleanup code here
}