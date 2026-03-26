#include "vertex.hpp"

Vertex::Vertex() : x(0), y(0), z(0);
Vertex::Vertex(double a, double b, double c) : x(a), y(b), z(c){}

Vertex operator+(const Vertex& v1, const Vertex& v2){
    return {v2.x + v1.x, v2.y + v1.y, v2.z + v1.z};
}
Vertex operator-(const Vertex& v1, const Vertex& v2){
    return {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
}

Vertex operator*(const Vertex& v1, const Vertex& v2){
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

Vertex Vertex::mid(const Vertex v){
    return {(x + v.x)/2, (y + v.y)/2, (z + v.z)/2};
}

double Vertex::dot(const Vertex v){
    return (x * v.x + y * v.y + z * v.z);
}

Vertex Vertex::minVx(Vertex v){
    double a = x < v.x ? x : v.x;
    double b = y < v.y ? y : v.y;
    double c = z < v.z ? z : v.z;

    return {a, b, c};
}

Vertex Vertex::maxVx(Vertex v){
    double a = x > v.x ? x : v.x;
    double b = y > v.y ? y : v.y;
    double c = z > v.z ? z : v.z;

    return {a, b, c};
}