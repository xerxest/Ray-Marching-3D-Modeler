#include <string>
#include "sdfBuffer.h"
// This Class is the front end for dealing with the SDF expression 

struct  SDFTreeNode
{
    std::string name;
    u_int32_t buffIndex;
    // Def in sdfDefintions.h
    u_int32_t type;
    float arguments[5];
    float postion[3];
    float scale[3];
};

struct SDTreeOperation
{
    // Def in sdfDefintions.h
    u_int16_t type;
    struct SDFTreeNode* dist1, dist2;
};

class sdfTree
{
private:
    SDTreeOperation root;
public:
    sdfTree(/* args */);
    ~sdfTree();
    void buildBuffer(sdfBuffer buffer);
};

sdfTree::sdfTree(/* args */)
{
}

sdfTree::~sdfTree()
{
}
