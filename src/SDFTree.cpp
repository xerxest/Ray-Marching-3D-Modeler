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

    case ShaderConfig::sdCylinder:
        return ShaderConfig::sdCylinder_glsl(m_bufferIndex);
        break;
    
    case ShaderConfig::sdCone:
        return ShaderConfig::sdCone_glsl(m_bufferIndex);
        break;
    
    case ShaderConfig::sdPlane:
        return ShaderConfig::sdPlane_glsl(m_bufferIndex);
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


std::string OperationNode::compileToGlsl() const
{   
    std::string leftChild = child(0)->compileToGlsl();
    std::string rightChild = child(1)->compileToGlsl();

    // TODO for loop

    switch (m_opp)
    {
    case ShaderConfig::opUnion:
        return ShaderConfig::opUnion_glsl(leftChild,rightChild);
        break;
    case ShaderConfig::opSubtract:
        return ShaderConfig::opSubtract_glsl(leftChild,rightChild);
        break;
    case ShaderConfig::opIntersect:
        return ShaderConfig::opIntersect_glsl(leftChild,rightChild);
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