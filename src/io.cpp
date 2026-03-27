#include "io.hpp"

void inputFile(const string& filename, vector<Vertex>& vertices, vector<Face>& faces, vector<Triangle>& triangles, Vertex& minBound, Vertex& maxBound) {
    string path = "../test/" + filename + ".obj";
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + path);
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;

        if (line.empty()) continue;

        stringstream ss(line);
        string type;
        ss >> type;

        // Vertex
        if (type == "v") {
            double x, y, z;

            if (!(ss >> x >> y >> z)) {
                throw runtime_error("Invalid vertex format at line " + to_string(lineNumber));
            }

            vertices.push_back({x, y, z});
            minBound = minBound.minVx(vertices.back());
            maxBound = maxBound.maxVx(vertices.back());
        }

        // Face
        else if (type == "f") {
            int i, j, k;

            if (!(ss >> i >> j >> k)) {
                throw runtime_error("Invalid face format at line " + to_string(lineNumber));
            }

            // validasi index
            if (i <= 0 || j <= 0 || k <= 0 || i > vertices.size() || j > vertices.size() || k > vertices.size()) {
                throw runtime_error("Face index out of range at line " + to_string(lineNumber));
            }

            faces.push_back({i, j, k});
            triangles.push_back({
                vertices[i - 1],
                vertices[j - 1],
                vertices[k - 1]
            });
        }

        else {
            throw runtime_error("Invalid line type at line " + to_string(lineNumber));
        }
    }
}

void writeFile(const string& filename, const MeshBuilder& mesh) {
    int i = 1;
    string path;
    while (true) {
        path = "../test/output_" + filename + "_" + to_string(i) + ".obj";

        if (!filesystem::exists(path)) {
            break;
        }

        i++;
    }
    ofstream out(path);

    for (auto& v : mesh.getVertices()) {
        out << "v " << v.x << " " << v.y << " " << v.z << "\n";
    }

    for (auto& f : mesh.getFaces()) {
        out << "f " << f.a << " " << f.b << " " << f.c << "\n";
    }

    out.close();
}