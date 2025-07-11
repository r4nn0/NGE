#ifndef NGESTD_HEADER
#define NGESTD_HEADER
#include <iostream>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <regex>
#include "Graphics/Sprite.h"
#include "Graphics/Object3D.h"
extern std::vector<std::reference_wrapper<Sprite>> SpritesToRender;
extern std::vector<std::reference_wrapper<Object3D>> ObjectsToRender;
static const std::wstring ar_alphabet = {
    0xfe8f, 0xfe95, 0xfe99, 0xfe9d,
    0xfea1, 0xfea5, 0xfeb1, 0xfeb5,
    0xfeb9, 0xfebd, 0xfec1, 0xfec5,
    0xfec9, 0xfecd, 0xfed1, 0xfed5,
    0xfed9, 0xfedd, 0xfee1, 0xfee5,
    0xfee9, 0xfef1, 0xfe8d, 0xfea9,
    0xfeab, 0xfead, 0xfeaf, 0xfeed,
    0xfe93, 0xfeef
};
static const std::wstring  ar_has_mi = {
    1576, 1578, 1579, 1580,
    1581, 1582, 1587, 1588,
    1589, 1590, 1591, 1592,
    1593, 1594, 1601, 1602,
    1603, 1604, 1605, 1606,
    1607, 1610
};
static const std::wstring  ar_has_no_mi = {
    1575, 1583, 1584,
    1585, 1586, 1608,
    1577, 1609
};

static const std::wstring  has_mi = {
    0xfe8f, 0xfe95, 0xfe99, 0xfe9d,
    0xfea1, 0xfea5, 0xfeb1, 0xfeb5,
    0xfeb9, 0xfebd, 0xfec1, 0xfec5,
    0xfec9, 0xfecd, 0xfed1, 0xfed5,
    0xfed9, 0xfedd, 0xfee1, 0xfee5,
    0xfee9, 0xfef1
};
static const std::wstring  has_no_mi = {
    0xfe8d, 0xfea9, 0xfeab,
    0xfead, 0xfeaf, 0xfeed,
    0xfe93, 0xfeef
};

std::wstring ar_fix(std::wstring);
std::vector<unsigned char*> LoadNGESprite(const char*, Sprite*);
void LoadModelsToMemory();
void LoadSpritesToMemroy();

struct CollisionBox{
    float left, right, top, bottom;
    CollisionBox();
    CollisionBox(float box) : left(box), right(box), top(box), bottom(box) {}
    CollisionBox(float l, float r, float u, float d): left(l), right(r), top(u), bottom(d){}
};

namespace ngestd{
    void DrawSetColor(glm::vec3);
    void DrawPoint(float, float);
    void DrawRectangle(glm::vec4, bool);
    glm::vec3 DrawGetColor();
};

#endif