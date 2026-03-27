#pragma once
#include <filesystem>
#include "helper.hpp"
#include "meshBuilder.hpp"
using namespace std;

void inputFile(const string& filename, vector<Vertex>& vertices, vector<Face>& faces, vector<Triangle>& triangles, Vertex& minBound, Vertex& maxBound);
void writeFile(const string& filename, const MeshBuilder& mesh);