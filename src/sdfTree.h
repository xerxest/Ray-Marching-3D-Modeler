
#ifndef H_SDFTREE
#define H_SDFTREE

#include <string>
#include <vector>
#include <iostream>
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
    SDFTree(/* args */);
    ~SDFTree(); 
    void addChild(SDFTree *node);
    // void removeChild(int childIndex);
    inline bool isRoot() const {return m_parent == nullptr;}
    // void setName(std::string name);
    // virtual int maxChildren() const;
    virtual std::string compileToGlsl() const;
    inline size_t childCount() const { return m_children.size(); }
    inline SDFTree *child(size_t index) { return m_children.at(index); }
    inline const SDFTree *child(size_t index) const { return m_children.at(index); }
    inline NodeType getType() const {return m_type;}
    inline void setType(NodeType type) {m_type = type;}
    inline void setName(std::string name) {m_name = name;}
    inline std::string getName() const {return m_name;}
    u_int64_t m_bufferIndex;

private:
    SDFTree *m_parent = nullptr;
    std::vector<SDFTree*> m_children;
    u_int8_t m_numChildren;
    NodeType m_type;
    std::string m_name;
};  


class SDFNode: public SDFTree 
{
    public:
        SDFNode(): SDFTree() {setType(SDFNodeType);}
        virtual std::string compileToGlsl() const override;
        inline const float* getPostion() const {return m_postion;}
        inline const float* getShape() const {return m_shapeOrSize;}
        inline void setNodeType(ShaderConfig::SDFType node) {m_nodeType = node;}
        inline void setPostiton(float pos1, float pos2, float pos3) { m_postion[0] = pos1; m_postion[1] = pos2; m_postion[2] = pos3; }
        inline void setShape(float f1, float f2, float f3) { m_shapeOrSize[0] = f1; m_shapeOrSize[1] = f2; m_shapeOrSize[2] = f3; }
        // virtual int maxChildren() const override;
    private:
        ShaderConfig::SDFType  m_nodeType;
        float m_postion[3];
        float m_shapeOrSize[3];
        // Mostly unused
        float radius;

};

class OperationNode: public SDFTree 
{
    public:
    	 OperationNode() : SDFTree() {setType(OperationNodeType); }
         virtual std::string compileToGlsl() const override;
         inline void setOperation(ShaderConfig::OperationType type) {m_opp = type;}
         inline ShaderConfig::OperationType operation() const { return m_opp; }
        //  virtual int maxChildren() const override;
         ShaderConfig::OperationType m_opp;
};

#endif // SDFTREE
