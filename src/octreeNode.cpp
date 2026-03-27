#include "octreeNode.hpp"

#include <algorithm>
#include <cmath>


void OctreeNode::subdivide(){
    this->isLeaf = false;
    Vertex mid = this->box.center;
    Vertex A = this->box.min; // atasnya A -> E
    Vertex G = this->box.max; // bawahnya G -> C

    this->children[0] = new OctreeNode(A, mid);
    this->children[1] = new OctreeNode({A.x, A.y, mid.z}, {mid.x, mid.y, G.z});
    this->children[2] = new OctreeNode({mid.x, A.y, mid.z}, {G.x, mid.y, G.z});
    this->children[3] = new OctreeNode({mid.x, A.y, A.z}, {G.x, mid.y, mid.z});
    this->children[4] = new OctreeNode({A.x, mid.y, A.z}, {mid.x, G.y, mid.z});
    this->children[5] = new OctreeNode({A.x, mid.y, mid.z}, {mid.x, G.y, G.z});
    this->children[6] = new OctreeNode(mid, G);
    this->children[7] = new OctreeNode({mid.x, mid.y, A.z}, {G.x, G.y, mid.z});
}