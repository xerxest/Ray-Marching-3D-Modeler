#include "ShaderBuilder.h"
#include "ShaderConfig.h"

void ShaderBuilder::writeNewShader(std::string dirPath)
{
    std::string glsl = m_rootPtr->compileToGlsl();

    std::string sceneDistGlsl = "float " + ShaderConfig::sceneDistName + "(vec3 _pos)\n {\nreturn " + glsl + ";\n}";

    std::cout << sceneDistGlsl << std::endl;

    std::ofstream outputFile(dirPath, std::ios::trunc);

    // Check if the file is successfully opened
    if (!outputFile.is_open())
    {
        std::cerr << "Unable to open the file." << std::endl;
        return;
    }

    outputFile << ShaderConfig::shaderFunctionsPath;

    int posSize = m_postionVec.size() / 4;

    int shapeSize = m_shapeOrSizeVec.size() / 4;

    int smoothSize = m_smoothValueVec.size() / 4;

    outputFile << "uniform vec4 " + ShaderConfig::u_positionName + "[" << ++posSize << "];\n";

    outputFile << "uniform vec4 " + ShaderConfig::u_shapeOrSizeName + "[" << ++shapeSize << "];\n";

    outputFile << "uniform vec4 " + ShaderConfig::u_smoothValue + "[" << ++smoothSize << "];\n";

    outputFile << sceneDistGlsl;

    outputFile.close();

    compileShader();
}

void ShaderBuilder::setUniforms()
{
    bgfx::setUniform(m_u_postions, m_postionVec.data(), m_postionVec.size() / 4);

    bgfx::setUniform(m_u_shapeOrSize, m_shapeOrSizeVec.data(), m_shapeOrSizeVec.size() / 4);

    bgfx::setUniform(m_u_smoothValue, m_smoothValueVec.data(), m_smoothValueVec.size() / 4);
}

void ShaderBuilder::createUniform()
{
    m_postionVec.clear();
    m_shapeOrSizeVec.clear();
    m_smoothValueVec.clear();

    std::queue<SDFTree *> sdQueue;

    int bufferIndex = 0;
    int smoothBufferIndex = 0;

    sdQueue.push(m_rootPtr->child(0).get());

    while (!sdQueue.empty())
    {
        SDFTree *currPtr = sdQueue.front();
        sdQueue.pop();
        int childIndex = 0;
        while (childIndex < currPtr->childCount())
        {
            sdQueue.push(currPtr->child(childIndex).get());
            childIndex++;
        }

        if (currPtr->getType() == SDFTree::SDFNodeType)
        {
            SDFNode *nodePtr = static_cast<SDFNode *>(currPtr);

            m_postionVec.push_back(nodePtr->m_p1);
            m_postionVec.push_back(nodePtr->m_p2);
            m_postionVec.push_back(nodePtr->m_p3);
            m_postionVec.push_back(0);

            m_shapeOrSizeVec.push_back(nodePtr->m_s1);
            m_shapeOrSizeVec.push_back(nodePtr->m_s2);
            m_shapeOrSizeVec.push_back(nodePtr->m_s3);
            m_shapeOrSizeVec.push_back(0);

            nodePtr->m_bufferIndex = bufferIndex;
            bufferIndex++;
        }
        else
        {
            OperationNode *opPtr = static_cast<OperationNode *>(currPtr);
            ShaderConfig::OperationType currType = opPtr->getOperationType();
            if (currType == ShaderConfig::opSmoothUnion || currType == ShaderConfig::opSmoothSubtraction || currType == ShaderConfig::opSmoothIntersection)
            {
                m_smoothValueVec.push_back(opPtr->m_smoothness);
                m_smoothValueVec.push_back(0);
                m_smoothValueVec.push_back(0);
                m_smoothValueVec.push_back(0);
                opPtr->m_smoothBufferIndex = smoothBufferIndex;
                smoothBufferIndex++;
            }
        }
    }

    m_u_postions = bgfx::createUniform(ShaderConfig::u_positionName.c_str(), bgfx::UniformType::Vec4, bufferIndex);

    m_u_shapeOrSize = bgfx::createUniform(ShaderConfig::u_shapeOrSizeName.c_str(), bgfx::UniformType::Vec4, bufferIndex);

    m_u_smoothValue = bgfx::createUniform(ShaderConfig::u_smoothValue.c_str(), bgfx::UniformType::Vec4, smoothBufferIndex);
}

void ShaderBuilder::createDebugScene()
{
    auto node1 = std::make_shared<SDFNode>();
    auto node2 = std::make_shared<SDFNode>();

    m_rootPtr->setName("Root");
    std::cout << "XERG createDebugScene" << std::endl;

    node1->setName("Test Box");
    node1->setNodeType(ShaderConfig::SDFType::udBox);

    node2->setNodeType(ShaderConfig::SDFType::sdSphere);
    node2->setName("Test Sphere");

    node1->setPosition(1.0f, 1.0f, 1.0f);
    node1->setShape(2.0f, 1.0f, 1.0f);

    node2->setPosition(4.0f, 1.0f, 0.0f);
    node2->setShape(1.0f, -1.0f, -1.0f);

    auto opp = std::make_shared<OperationNode>();

    opp->setName("opUnion");
    opp->setOperation(ShaderConfig::OperationType::opUnion);

    opp->addChild(node1);
    opp->addChild(node2);

    m_rootPtr->addChild(opp);

    createUniform();
    writeNewShader(ShaderConfig::sceneShaderName);
    setUniforms();
}

void ShaderBuilder::compileShader()
{
    const std::filesystem::path m_shaderPath(ShaderConfig::shaderBuildPath);

#if defined(_WIN32) || defined(_WIN64)
    const char *command = "make TARGET=0;";
#elif defined(__APPLE__) || defined(__MACH__)
    const char *command = "make TARGET=5;";
#elif defined(__linux__)
    const char *command = "make TARGET=4;";
#elif defined(__unix__)
    const char *command = "make TARGET=4;";
#else
    const char *command = "make TARGET=4;";
#endif

    std::filesystem::current_path(m_shaderPath);

    int result = std::system(command);

    const std::filesystem::path buildPath("../../build");
    std::filesystem::current_path(buildPath);

    if (result == 0)
    {
        // The command executed successfully
        std::cout << "Command executed successfully.\n";
    }
    else
    {
        // There was an error in executing the command
        std::cerr << "Error executing command.\n";
    }
}

void ShaderBuilder::setNodeUniforms(std::shared_ptr<SDFTree> node)
{
    if (!node->isRoot() && !m_shaderReCompFlag)
    {
        if (node->getType() == SDFTree::SDFNodeType)
        {
            SDFNode *nodePtr = static_cast<SDFNode *>(node.get());

            size_t index = nodePtr->m_bufferIndex * 4;

            m_postionVec[index++] = nodePtr->m_p1;
            m_postionVec[index++] = nodePtr->m_p2;
            m_postionVec[index] = nodePtr->m_p3;

            index = nodePtr->m_bufferIndex * 4;

            m_shapeOrSizeVec[index++] = nodePtr->m_s1;
            m_shapeOrSizeVec[index++] = nodePtr->m_s2;
            m_shapeOrSizeVec[index] = nodePtr->m_s3;
        }
        else
        {
            OperationNode *oppPtr = static_cast<OperationNode *>(node.get());
            ShaderConfig::OperationType currType = oppPtr->getOperationType();
            if (currType == ShaderConfig::opSmoothUnion || currType == ShaderConfig::opSmoothSubtraction || currType == ShaderConfig::opSmoothIntersection)
            {
                m_smoothValueVec[oppPtr->m_smoothBufferIndex * 4] = oppPtr->m_smoothness;
            }
        }
    }
}

bool ShaderBuilder::updateScene()
{
    if (m_shaderReCompFlag)
    {
        createUniform();

        writeNewShader(ShaderConfig::sceneShaderName);

        setUniforms();

        m_shaderReCompFlag = false;

        return true;
    }
    else
    {
        setUniforms();
        return false;
    }
}

ShaderBuilder::ShaderBuilder(std::shared_ptr<SDFTree> root)
{
    m_rootPtr = root;
}

ShaderBuilder::~ShaderBuilder()
{
}
