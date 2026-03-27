#include <iostream>
#include <chrono>
#include <thread>
#include "octreeNode.hpp"
#include "meshBuilder.hpp"
#include "io.hpp"

struct Checker{
    long nodeBuild;
    long notChecked;

    Checker() : nodeBuild(0), notChecked(0) {};
};

void buildOctree(OctreeNode* node, int depth, int maxDepth, vector<Triangle> &triangles, vector<Cube> &voxel, vector<Checker> &c) {
    
    // cek apakah cube ini kena triangle
    bool intersect = false;
    for(auto& tri : triangles){
        if(Intersects(tri, node->box)){
            intersect = true;
            break;
        }
    }
    
    if(!intersect){ // jika tidak berpotongan, maka tidak perlu lanjut telusuri
        c[depth-1].notChecked++;
        return; // kosong, stop
    }
    
    if(depth == maxDepth){ // Cek apakah sudah voxel terkecil
        node->isFilled = true; // kalau sudah masukkan voxel ke list
        voxel.push_back(node->box);
        return;
    }
    
    node->subdivide(); // Kalau berpotongan, tapi belum voxel terkecil -> bagi 8
    
    for(int i = 0; i < 8; i++){
        c[depth].nodeBuild++;
        buildOctree(node->children[i], depth + 1, maxDepth, triangles, voxel, c);
    }
}

int main() {
    vector<Vertex> vertices;
    vector<Cube> voxels;
    vector<Triangle> triangles;
    vector<Face> faces;
    double maxD = numeric_limits<double>::max();
    double minD = -maxD;
    Vertex* minVx = new Vertex(maxD, maxD, maxD);
    Vertex* maxVx = new Vertex(minD, minD, minD);
    int maxDepth = 0;
    
    string filename;
    cout << "Pilih file: ";
    cin >> filename;
    
    try {
        inputFile(filename, vertices, faces, triangles, *minVx, *maxVx);
        cout << "Masukan depth maksimum: ";
        cin >> maxDepth;
        cout << "=========INPUT STAT=========" << endl;
        cout << "Depth: " << maxDepth << endl;
        cout << "Vertices: " << vertices.size() << endl;
        cout << "Faces: " << triangles.size() << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    vector<Checker> stat(maxDepth);
    
    auto start = chrono::high_resolution_clock::now();

    // =========== MAIN ALGORITHM ===========
    try {
        OctreeNode* root = new OctreeNode(*minVx, *maxVx);
        buildOctree(root, 0, maxDepth, triangles, voxels, stat);
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    // ============= END MAIN ================
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    auto mainTime = duration.count();
    
    start = chrono::high_resolution_clock::now();
    double voxelSize = voxels[0].half;
    MeshBuilder mesh(voxelSize);
    for (auto node : voxels) {
        addVoxelCube(node.min, node.max, mesh);
    }
    
    cout << "\n\n=========OUTPUT STAT=========" << endl;

    writeFile(filename, mesh); 
    
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    auto writeTime = duration.count();
    
    cout << "Main time: " << mainTime << " seconds" << endl;
    cout << "Write time: " << writeTime << " seconds" << endl;
    
    cout << "\nJumlah Voxel: " << voxels.size() << endl;
    cout << "Jumlah Vertices: " << mesh.getVertices().size() << endl;
    cout << "Jumlah Faces: " << mesh.getFaces().size()  << endl << endl;

    cout << "==> Banyaknya Node" << endl;
    for(int i = 0; i < maxDepth; i++){
        cout << stat[i].nodeBuild << "  : banyaknya node dengan depth " << i+1 <<" yang terbentuk"<< endl;
    }
    cout << "\n==> Tidak Perlu Ditelusuri" << endl;
    for(int i = 0; i < maxDepth; i++){
        cout << stat[i].notChecked << "  : banyaknya node dengan depth " << i+1 <<" yang tidak perlu ditelusuri"<< endl << endl;
    }
    
    return 0;
}