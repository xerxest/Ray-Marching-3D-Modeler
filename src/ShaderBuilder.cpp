#include "ShaderBuilder.h"
#include "ShaderConfig.h"

void ShaderBuilder::writeNewShader(std::string dirPath,SDFTree &scene)
{
    std::string glsl = scene.compileToGlsl();

    std::string sceneDistGlsl =  "float "+ShaderConfig::sceneDistName+"(vec3 _pos)\n {\nreturn "+glsl+";\n}";

    std::cout << sceneDistGlsl << std::endl;

    std::ofstream outputFile(dirPath, std::ios::trunc);

       // Check if the file is successfully opened
    if (!outputFile.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
        return ;
    }

    outputFile << ShaderConfig::shaderFunctionsPath;

    int posSize = m_postionVec.size()/4;

    int shapeSize = m_shapeOrSizeVec.size()/4;

    outputFile << "uniform vec4 "+ShaderConfig::u_positionName+"["<< ++posSize << "];\n";

    outputFile << "uniform vec4 "+ShaderConfig::u_shapeOrSizeName+"[" << ++shapeSize <<"];\n";

    outputFile << sceneDistGlsl;

    outputFile.close();

    compileShader();
}

void ShaderBuilder::setUniforms()
{
    bgfx::setUniform(m_u_postions,m_postionVec.data(),m_postionVec.size()/4);

    bgfx::setUniform(m_u_shapeOrSize,m_shapeOrSizeVec.data(),m_shapeOrSizeVec.size()/4);
}

void ShaderBuilder::createUniform(SDFTree *scene)
{
    m_postionVec.clear();
    m_shapeOrSizeVec.clear();
    
    std::queue<SDFTree*> sdQueue;

    int bufferIndex = 0;

    sdQueue.push(scene->child(0));

    while (!sdQueue.empty())
    {   
        SDFTree* currPtr = sdQueue.front();
        sdQueue.pop();
        int childIndex = 0;
        while (childIndex < currPtr->childCount())
        {
            sdQueue.push(currPtr->child(childIndex));
            childIndex++;
            std::cout << childIndex << std::endl;
        }

        if(currPtr->getType() == SDFTree::SDFNodeType)
        {
            SDFNode* nodePtr = static_cast<SDFNode*>(currPtr);
            const float* postionPtr =  nodePtr->getPostion();

            m_postionVec.push_back(*postionPtr);
            m_postionVec.push_back(*(++postionPtr));
            m_postionVec.push_back(*(++postionPtr));
            m_postionVec.push_back(0);

            const float* sizeOrshapePtr = nodePtr->getShape();

            m_shapeOrSizeVec.push_back(*sizeOrshapePtr);
            m_shapeOrSizeVec.push_back(*(++sizeOrshapePtr));
            m_shapeOrSizeVec.push_back(*(++sizeOrshapePtr));
            m_shapeOrSizeVec.push_back(0);

            nodePtr->m_bufferIndex = bufferIndex;
            bufferIndex++;

            m_sdVec.push_back(nodePtr);

        }

    }

    m_u_postions = bgfx::createUniform(ShaderConfig::u_positionName.c_str(), bgfx::UniformType::Vec4,bufferIndex);

    m_u_shapeOrSize = bgfx::createUniform(ShaderConfig::u_shapeOrSizeName.c_str(), bgfx::UniformType::Vec4,bufferIndex);
}


void ShaderBuilder::createDebugScene(SDFTree &scene)
{
    SDFNode* node1 = new SDFNode();
    SDFNode* node2 = new SDFNode();

    scene.setName("Root");

    node1->setName("Test Box");
    node1->setNodeType(ShaderConfig::SDFType::udBox);

    node2->setNodeType(ShaderConfig::SDFType::sdSphere);
    node2->setName("Test Sphere");

    node1->setPostiton(1.0f,1.0f,1.0f);
    node1->setShape(2.0f,1.0f,1.0f);

    node2->setPostiton(4.0f,1.0f,0.0f);
    node2->setShape(1.0f,-1.0f,-1.0f);

    OperationNode* opp = new OperationNode();

    opp->setName("opUnion");

    opp->setOperation(ShaderConfig::OperationType::opUnion);

    opp->addChild(node1);
    opp->addChild(node2);

    scene.addChild(opp);

    createUniform(&scene);

    writeNewShader(ShaderConfig::sceneShaderName,scene);

    setUniforms();
}

void ShaderBuilder::compileShader()
{
    const std::filesystem::path m_shaderPath(ShaderConfig::shaderBuildPath);

    // TODO mutli plafrom

    const char* command = "make TARGET=5;";

    std::filesystem::current_path(m_shaderPath);

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

ShaderBuilder::ShaderBuilder(/* args */)
{
}

ShaderBuilder::~ShaderBuilder()
{
}
