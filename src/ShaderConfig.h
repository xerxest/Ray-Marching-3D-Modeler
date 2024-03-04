#pragma once
#include <string>
namespace ShaderConfig
{
    /*----  Uniforms Names ----*/ 

    const std::string u_positionName = "u_postions";
    const std::string u_shapeOrSizeName = "u_shapeOrSize";
    const std::string u_smoothValue = "u_smoothValue";
    
    /*----  SDF Types ----*/ 

    enum SDFType
        {
            sdSphere,
            udBox,
            udRoundBox,
            sdTorus,
            sdCone,
            sdHexPrism,
            sdTriPrism
        };
    
    /*----  Operation Types ----*/ 
    
    enum OperationType
        {
           opUnion,
           opSubtract,
           opIntersect,
           opSmoothUnion,
           opSmoothSubtraction,
           opSmoothIntersection,
        };

    /*----  Operation Functions ----*/ 

    inline std::string opUnion_glsl(std::string leftNode, std::string rightNode)
    {
        return "opUnion("+leftNode+"," +rightNode+")";
    }

    inline std::string opSubtract_glsl(std::string leftNode, std::string rightNode)
    {
        return "opSubtract("+leftNode+"," +rightNode+")";
    }

    inline std::string opIntersect_glsl(std::string leftNode, std::string rightNode)
    {
        return "opIntersect("+leftNode+"," +rightNode+")";
    }

    inline std::string opSmoothUnion_glsl(std::string leftNode, std::string rightNode, int bufferIndex)
    {
        return "opSmoothUnion("+leftNode+"," +rightNode+","+u_smoothValue+"["+std::to_string(bufferIndex)+"].x )";
    }

    inline std::string opSmoothSubtraction_glsl(std::string leftNode, std::string rightNode, int bufferIndex)
    {
        return "opSmoothSubtraction("+leftNode+"," +rightNode+","+u_smoothValue+"["+std::to_string(bufferIndex)+"].x )";
    }

    inline std::string opSmoothIntersection_glsl(std::string leftNode, std::string rightNode, int bufferIndex)
    {
        return "opSmoothIntersection("+leftNode+"," +rightNode+","+u_smoothValue+"["+std::to_string(bufferIndex)+"].x )";
    }
    
    /*----  SDF Functions ---- */ 

    inline std::string sdSphere_glsl(int index)
    {
        return "sdSphere(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].x)";
    }

    inline std::string udBox_glsl(int index)
    {
        return "udBox(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xyz)";
    }

    inline std::string udRoundBox_glsl(int index)
    {
        return "udRoundBox(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xyz, 1.0)";
    }
    
    inline std::string sdTorus_glsl(int index)
    {
        return "sdTorus(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xy)";
    }

    inline std::string sdCone_glsl(int index)
    {
        return "sdCone(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xy, u_shapeOrSize["+std::to_string(index)+"].z)";
    }

    inline std::string sdHexPrism_glsl(int index)
    {
        return "sdHexPrism(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xy)";
    }

    inline std::string sdTriPrism_glsl(int index)
    {
        return "sdTriPrism(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xy)";
    }

    /*----  Shader Name and Path ----*/ 

    const std::string shaderFunctionsPath = "#include \"../src/shaders/iq_sdf.sc\"\n";

    const std::string sceneShaderName = "RuntimeSDFData.sh";

    const std::string shaderBuildPath = "../src/shaders/";

    // Name used in shader file
    const std::string sceneDistName = "RootDist";


} // ShaderConfig