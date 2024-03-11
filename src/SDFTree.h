
#ifndef H_SDFTREE
#define H_SDFTREE

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "ShaderConfig.h"


class SDFTree
{
public:
    enum NodeType
    {
        OperationNodeType,
        SDFNodeType
    };
    
public:
    SDFTree(/* args */) noexcept;
    ~SDFTree(); 
    void addChild(std::shared_ptr<SDFTree> node);
    void removeChild(std::shared_ptr<SDFTree> node);
    inline bool isRoot() const {return m_parent == nullptr;}
    virtual std::string compileToGlsl() const;
    inline size_t childCount() const { return m_children.size(); }
    inline std::shared_ptr<SDFTree> child(size_t index) { return m_children.at(index); }
    inline const std::shared_ptr<SDFTree> child(size_t index) const { return m_children.at(index); }
    inline NodeType getType() const {return m_type;}
    inline void setType(NodeType type) {m_type = type;}
    inline void setName(std::string name) {m_name = name;}
    inline std::string getName() const {return m_name;}
    inline SDFTree* getParent() {return m_parent;}
    u_int64_t m_bufferIndex;

private:
    SDFTree* m_parent = nullptr;
    std::vector<std::shared_ptr<SDFTree>> m_children;
    u_int8_t m_numChildren;
    NodeType m_type;
    std::string m_name;
};  


class SDFNode: public SDFTree 
{
    public:
        SDFNode(): SDFTree() {setType(SDFNodeType);}
        std::string compileToGlsl() const override;
        inline ShaderConfig::SDFType getNodeType() const {return m_nodeType;}
        inline void setNodeType(ShaderConfig::SDFType node) {m_nodeType = node;}
        inline void setPosition(float pos1, float pos2, float pos3) { m_p1 = pos1; m_p2 = pos2; m_p2 = pos3; }
        inline void setShape(float f1, float f2, float f3) { m_s1 = f1; m_s2 = f2; m_s3 = f3; }
        
        float m_p1;
        float m_p2;
        float m_p3;

        float m_s1;
        float m_s2;
        float m_s3;

    private:
        ShaderConfig::SDFType  m_nodeType;
        // Mostly unused
        float radius;

};

class OperationNode: public SDFTree 
{
    public:
    	OperationNode() : SDFTree() {setType(OperationNodeType); }
        virtual std::string compileToGlsl() const override;
        inline void setOperation(ShaderConfig::OperationType type) {m_opp = type;}
        inline ShaderConfig::OperationType getOperationType() const { return m_opp; }
        float m_smoothness = 0;
        size_t m_smoothBufferIndex;
    private:
        ShaderConfig::OperationType m_opp;
        std::string wrapAllChildrenWithOpp(std::string (*func)(std::string,std::string)) const;
        std::string wrapAllChildrenWithSmoothOpp(std::string (*func)(std::string,std::string,int)) const;
};

#endif // SDFTREE
