
#ifndef SHADERBUILDER_H
#define SHADERBUILDER_H

#include "bgfx_utils.h"
#include "SDFTree.h"
#include <memory>
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <queue>
#include <set>

class ShaderBuilder
{
public:
    ShaderBuilder(SDFTree* root);
    ~ShaderBuilder();
    void writeNewShader(std::string dirPath);
    inline void setShaderPath(std::string path) {m_shaderPath = path;}
    void createDebugScene();
    void setNodeUniforms(SDFTree* node);
    inline void markForRecompile() {m_shaderReCompFlag = true;}
    bool updateScene();
private:
    SDFTree* m_rootPtr;
    bgfx::UniformHandle m_u_postions;
    bgfx::UniformHandle m_u_shapeOrSize;
    bgfx::UniformHandle m_u_smoothValue;

    std::vector<float> m_postionVec;
    std::vector<float> m_shapeOrSizeVec;
    std::vector<float> m_smoothValueVec;

    std::string m_shaderPath;
    bool m_shaderReCompFlag = false;

    void compileShader();
    void setUniforms();
    void createUniform();
};

#endif // SHADERBUILDER

