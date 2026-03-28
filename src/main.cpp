#include <iostream>
#include <chrono>
#include <thread>
#include "octreeNode.hpp"
#include "faceBuilder.hpp"
#include "io.hpp"

struct Checker{
    long nodeBuild;
    long notChecked;

    Checker() : nodeBuild(0), notChecked(0) {};
};

void buildOctree(OctreeNode* node, int depth, int maxDepth, vector<Triangle> triangles, vector<Cube> &voxel, vector<Checker> &c) {
    
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
        cout << "\n=========INPUT STAT=========" << endl;
        cout << "Depth: " << maxDepth << endl;
        cout << "Vertices: " << vertices.size() << endl;
        cout << "Faces: " << triangles.size() << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }


    // =========== Inisisasi untuk penyimpan data tiap thread dan hasil akhir ===========
    vector<Cube> voxels;
    vector<Checker> stats(maxDepth);
    vector<vector<Cube>> threadVoxels(8);
    vector<vector<Checker>> threadStats(8, vector<Checker>(maxDepth));
    
    auto start = chrono::high_resolution_clock::now();

    // =========== MAIN ALGORITHM ===========
    try {
        OctreeNode* root = new OctreeNode(*minVx, *maxVx);
        root->subdivide();
        thread t1(buildOctree, root->children[0], 1, maxDepth, triangles, ref(threadVoxels[0]), ref(threadStats[0]));
        thread t2(buildOctree, root->children[1], 1, maxDepth, triangles, ref(threadVoxels[1]), ref(threadStats[1]));
        thread t3(buildOctree, root->children[2], 1, maxDepth, triangles, ref(threadVoxels[2]), ref(threadStats[2]));
        thread t4(buildOctree, root->children[3], 1, maxDepth, triangles, ref(threadVoxels[3]), ref(threadStats[3]));
        thread t5(buildOctree, root->children[4], 1, maxDepth, triangles, ref(threadVoxels[4]), ref(threadStats[4]));
        thread t6(buildOctree, root->children[5], 1, maxDepth, triangles, ref(threadVoxels[5]), ref(threadStats[5]));
        thread t7(buildOctree, root->children[6], 1, maxDepth, triangles, ref(threadVoxels[6]), ref(threadStats[6]));
        thread t8(buildOctree, root->children[7], 1, maxDepth, triangles, ref(threadVoxels[7]), ref(threadStats[7]));

        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();
        // buildOctree(root, 0, maxDepth, triangles, voxels, stats);
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    // gabung hasil tiap thread
    for (int i = 0; i < 8; i++) {
        voxels.insert(voxels.end(), threadVoxels[i].begin(), threadVoxels[i].end());
        for (int j = 0; j < maxDepth; j++) {
            stats[j].nodeBuild += threadStats[i][j].nodeBuild;
            stats[j].notChecked += threadStats[i][j].notChecked;
        }
    }
    stats[0].nodeBuild = 8;
    // ============= END MAIN ================
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    auto mainTime = duration.count();
    
    start = chrono::high_resolution_clock::now();
    double voxelSize = voxels[0].half;

    FaceBuilder face(voxelSize);
    for (auto node : voxels) {
        addVoxelCube(node.min, node.max, face);
    }
    
    cout << "\n=========OUTPUT STAT=========" << endl;

    writeFile(filename, face); 
    
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    auto writeTime = duration.count();
    
    cout << "Main time: " << mainTime << " seconds" << endl;
    cout << "Write time: " << writeTime << " seconds" << endl;
    
    cout << "\nJumlah Voxel: " << voxels.size() << endl;
    cout << "Jumlah Vertices: " << face.getVertices().size() << endl;
    cout << "Jumlah Faces: " << face.getFaces().size()  << endl << endl;

    cout << "==> Banyaknya Node" << endl;
    for(int i = 0; i < maxDepth; i++){
        cout << stats[i].nodeBuild << "  : banyaknya node dengan depth " << i+1 << " yang terbentuk"<< endl;
    }
    cout << "\n==> Tidak Perlu Ditelusuri" << endl;
    for(int i = 0; i < maxDepth; i++){
        cout << stats[i].notChecked << "  : banyaknya node dengan depth " << i+1 << " yang tidak perlu ditelusuri"<< endl;
    }
    
    return 0;
}