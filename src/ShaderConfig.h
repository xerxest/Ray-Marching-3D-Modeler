#pragma once
#include <string>
namespace ShaderConfig
{
    /*----  Uniforms Names ----*/ 

    const std::string u_positionName = "u_postions";
    const std::string u_shapeOrSizeName = "u_shapeOrSize";

    /*----  SDF Types ----*/ 

    enum SDFType
        {
            sdSphere,
            udBox,
            udRoundBox,
            sdBox,
            sdTorus,
            sdCylinder,
            sdCone,
            sdPlane,
            sdHexPrism,
            sdTriPrism
        };
    
    /*----  Operation Types ----*/ 
    
    enum OperationType
        {
           opUnion,
           opSubtract,
           opIntersect,
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
    
    /*----  SDF Functions ---- TODO */ 

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
        return "udBox(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xyz)";
    }

    inline std::string sdBox_glsl(int index)
    {
        return "udBox(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xyz)";
    }

    inline std::string sdTorus_glsl(int index)
    {
        return "udBox(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xyz)";
    }

    inline std::string sdCylinder_glsl(int index)
    {
        return "udBox(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xyz)";
    }

    inline std::string sdCone_glsl(int index)
    {
        return "udBox(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xyz)";
    }

    inline std::string sdPlane_glsl(int index)
    {
        return "udBox(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xyz)";
    }

    inline std::string sdHexPrism_glsl(int index)
    {
        return "udBox(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xyz)";
    }

    inline std::string sdTriPrism_glsl(int index)
    {
        return "udBox(_pos + u_postions["+std::to_string(index)+"].xyz,u_shapeOrSize["+std::to_string(index)+"].xyz)";
    }

    /*----  Shader Name and Path ----*/ 

    const std::string shaderFunctionsPath = "#include \"../src/shaders/iq_sdf.sh\"\n";

    const std::string sceneShaderName = "RuntimeSDFData.sh";

    const std::string shaderBuildPath = "../src/shaders/";

    // Name used in shader file
    const std::string sceneDistName = "RootDist";


} // ShaderConfig