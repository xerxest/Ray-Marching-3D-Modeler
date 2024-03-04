#pragma once
#include "imgui/imgui.h"
#include <array>

class Panel
{
private:
    /* data */
public:
    std::array<const char *, 7> m_sdStrings = {
        "Sphere",
        "Box",
        "Round Box",
        "Torus",
        "Cone",
        "HexPrism",
        "TriPrism"};

    std::array<const char *, 6> m_opStrings = {
        "opUnion",
        "opSubtract",
        "opIntersect",
        "opSmoothUnion",
        "opSmoothSubtraction",
        "opSmoothIntersection",
    };

    static float m_screenWidth;
    static float m_screenHeight;
    void setStyle();
};
