const float SDSPHERE    = 0.0;
const float UDROUNDBOX  = 1.0;
const float SDBOX       = 2.0;
const float SDTORUS     = 3.0;
const float SDCYLINDER  = 4.0;
const float SDCONE      = 5.0;
const float SDPLANE     = 6.0;
const float SDHEXPRISM  = 7.0;
const float SDTRIPRISM  = 8.0;

const float OPUNION     = 9.0;
const float OPSUBTRACT  = 10.0;
const float OPINTERSECT = 11.0;

const float OPP_TYPE     = 12.0;
const float SDNODE_TYPE  = 13.0;


struct SDF_Node
{
    float type;
    float positon[3];
    float size[3];
};
