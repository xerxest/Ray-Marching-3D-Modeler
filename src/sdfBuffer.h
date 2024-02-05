#pragma once
#include "shaders/sdfDefinitions.h"
#include "bgfx_utils.h"
#include <memory>
#include <vector>
#include <array>
#include <string>
#include <iostream>

// TODO Pack Data
struct sdfDataVec 
{
    std::vector<std::array<float,4>> type, position, size;
};

class sdfBuffer
{
private:
    // TODO Pack Data
    struct sdfDataVec sdBuffer;
    std::vector<std::array<float,4>> sdfOpperationDataVec;
    std::vector<std::array<float,4>> postFixDataVec;
    bgfx::UniformHandle typeHandle;
    bgfx::UniformHandle posHandle;
    bgfx::UniformHandle sizeHandle;

    bgfx::UniformHandle oppHandle; 
    bgfx::UniformHandle postFixHandle;

    std::vector<float> sdTypeData;
    std::vector<float> sdPosData;
    std::vector<float> sdSizeData;
    std::vector<float> oppData;
    std::vector<float> postFixData;
    void writeVec4Data(std::string typeName, std::ofstream& stream);
public:
    sdfBuffer(/* args */);
    ~sdfBuffer();
    void appendNode(float type, float position[3], float size[3]);
    void setNode(float type, float position[3], float size[3], int index);
    void addOperation(float type);
    void setUniforms();
    void initUniforms();
    void setBuffers();
    // Temporary work around
    void writeToHeader();
    void recompileShader();
};