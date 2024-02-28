
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
private:
    bgfx::UniformHandle m_u_postions;
    bgfx::UniformHandle m_u_shapeOrSize;

    std::vector<float> m_postionVec;
    std::vector<float> m_shapeOrSizeVec;

    std::vector<SDFTree*> m_sdVec; 

    std::string m_shaderPath;
public:
    ShaderBuilder(/* args */);
    ~ShaderBuilder();
    void writeNewShader(std::string dirPath,SDFTree &scene);
    inline void setShaderPath(std::string path) {m_shaderPath = path;}
    void createDebugScene(SDFTree &scene);
    void setUniforms();
private:
    void compileShader();
    void createUniform(SDFTree *scene);
};

#endif // SHADERBUILDER

