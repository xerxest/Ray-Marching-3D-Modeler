#include "sdfBuffer.h"
#include <iostream>
#include <fstream>
#include <filesystem>

sdfBuffer::sdfBuffer(/* args */)
{
}

sdfBuffer::~sdfBuffer()
{
}

void sdfBuffer::appendNode(float type, float positon[3], float size[3])
{
    sdBuffer.type.push_back({type,-1.0f,-1.0f,-1.0f});
    sdBuffer.position.push_back({positon[0],positon[1],positon[2], -1.0f});
    sdBuffer.size.push_back({size[0],size[1],size[2], -1.0f});

    postFixDataVec.push_back({SDNODE_TYPE,-1.0f,-1.0f,-1.0f});
}

void sdfBuffer::setNode(float type, float position[3], float size[3], int index)
{
    sdBuffer.type[index] = {type,-1.0f,-1.0f,-1.0f};
    sdBuffer.position[index] = {position[0], position[1], position[2]};
    sdBuffer.size[index] = {size[0], size[1], size[2]};
}

void sdfBuffer::addOperation(float type)
{
    sdfOpperationDataVec.push_back({type,-1.0f,-1.0f,-1.0f});
    postFixDataVec.push_back({OPP_TYPE,-1.0f,-1.0f,-1.0f});
}


void sdfBuffer::initUniforms()
{   
    typeHandle = bgfx::createUniform("u_SD_type", bgfx::UniformType::Vec4, sdBuffer.type.size()); 
    posHandle = bgfx::createUniform("u_SD_position", bgfx::UniformType::Vec4, sdBuffer.position.size());  
    sizeHandle = bgfx::createUniform("u_SD_size", bgfx::UniformType::Vec4, sdBuffer.size.size()); 
    oppHandle = bgfx::createUniform("u_opps", bgfx::UniformType::Vec4, sdfOpperationDataVec.size()); 
    postFixHandle = bgfx::createUniform("u_postFix", bgfx::UniformType::Vec4, postFixDataVec.size());
}

void sdfBuffer::setBuffers()
{
    sdTypeData.clear();
    sdPosData.clear();
    sdSizeData.clear();
    oppData.clear();
    postFixData.clear();

    for (const auto& array : sdBuffer.type) {
        for (float value : array) {
            sdTypeData.push_back(value);
        }
    }


    for (const auto& array : sdBuffer.position) {
        for (float value : array) {
            sdPosData.push_back(value);
        }
    }

    // size

    for (const auto& array : sdBuffer.size) {
        for (float value : array) {
            sdSizeData.push_back(value);
        }
    }


    for (const auto& array : sdfOpperationDataVec) {
        for (float value : array) {
            oppData.push_back(value);
        }
    }


    for (const auto& array : postFixDataVec) {
        for (float value : array) {
            postFixData.push_back(value);
        }
    }

}

//TODO more efficient method
void sdfBuffer::setUniforms()
{
    bgfx::setUniform(typeHandle, sdTypeData.data(),  sdBuffer.type.size());
    // std::cout << "TypeDataArr size: " << sizeof(typeDataArr) / sizeof(typeDataArr[0]) << "sdBuffer size:" << sdBuffer.type.size() << std::endl;
    bgfx::setUniform(posHandle, sdPosData.data(), sdBuffer.position.size());
    bgfx::setUniform(sizeHandle, sdSizeData.data(), sdBuffer.size.size());

    bgfx::setUniform(oppHandle, oppData.data(), sdfOpperationDataVec.size());
    bgfx::setUniform(postFixHandle, postFixData.data(), postFixDataVec.size());

}


void sdfBuffer::writeToHeader()
{
    char filename[] = "RuntimeSDFData.sh";

    std::string sdDataNames[] = {
        "u_SD_type",
        "u_SD_position",
        "u_SD_size",
        "u_opps",
        "u_postFix",
    };

    std::string constStrings[] = {
    "SDSPHERE",
    "UDROUNDBOX",
    "SDBOX",
    "SDTORUS",
    "SDCYLINDER",
    "SDCONE",
    "SDPLANE",
    "SDHEXPRISM",
    "SDTRIPRISM",
    "OPUNION",
    "OPSUBTRACT",
    "OPINTERSECT",
    "OPP_TYPE",
    "SDNODE_TYPE",
    };

    std::ofstream outputFile(filename, std::ios::trunc);

       // Check if the file is successfully opened
    if (!outputFile.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
        return ;
    }

    char varType[] = "const vec4 ";

    for (int i =0; i < 5; i++)
    {
        outputFile << varType;
        outputFile << sdDataNames[i];
        outputFile << "[] = {";
        writeVec4Data(sdDataNames[i],outputFile);
        outputFile <<"}; \n";
    }

    outputFile << "const int postFixLength = " << postFixDataVec.size() << ";";

    outputFile.close();
}

void sdfBuffer::recompileShader()
{

    const std::filesystem::path shaderPath("../src/shaders/");

    const char* command = "make TARGET=5;";

    std::filesystem::current_path(shaderPath);

    int result = std::system(command);

    const std::filesystem::path buildPath("../../build");
    std::filesystem::current_path(buildPath);

    if (result == 0) {
            // The command executed successfully
            std::cout << "Command executed successfully.\n";
    } else {
            // There was an error in executing the command
            std::cerr << "Error executing command.\n";
    }

}

void sdfBuffer::writeVec4Data(std::string typeName, std::ofstream& stream)
{   
    int i = 0;
    if(typeName == "u_SD_type")
    {
        while (i < sdTypeData.size())
        {
            stream << "vec4(" << sdTypeData[i++] << "," << sdTypeData[i++] << "," << sdTypeData[i++] << "," << sdTypeData[i++] << "),";
        }
    }
    else if (typeName == "u_SD_position")
    {

          while (i < sdPosData.size())
        {
            stream << "vec4(" << sdPosData[i++] << "," << sdPosData[i++] << "," << sdPosData[i++] << "," << sdPosData[i++] << "),";
        }

    }
    else if (typeName == "u_SD_size")
    {

          while (i < sdSizeData.size())
        {
            stream << "vec4(" << sdSizeData[i++] << "," << sdSizeData[i++] << "," << sdSizeData[i++] << "," << sdSizeData[i++] << "),";
        }

    }
    else if (typeName == "u_opps")
    {

          while (i < oppData.size())
        {
            stream << "vec4(" << oppData[i++] << "," << oppData[i++] << "," << oppData[i++] << "," << oppData[i++] << "),";
        }

    }
    else if (typeName == "u_postFix")
    {
         while (i < postFixData.size())
        {
            stream << "vec4(" << postFixData[i++] << "," << postFixData[i++] << "," << postFixData[i++] << "," << postFixData[i++] << "),";
        }
        
    }
}
