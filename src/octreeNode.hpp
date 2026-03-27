#pragma once

#include <memory>
#include <vector>
#include "helper.hpp"
using namespace std;

class OctreeNode{
    public:
        Cube box;
        bool isLeaf;
        bool isFilled;
        OctreeNode* children[8];

        OctreeNode(Vertex min, Vertex max){
            box.min = min;
            box.max = max;
            box.center = min.mid(max);
            box.half = abs(min.x - max.x)/2;
            isLeaf = true;
            isFilled = false;
            for (int i = 0; i < 8; i++) children[i] = nullptr;
        }
        void subdivide();
};