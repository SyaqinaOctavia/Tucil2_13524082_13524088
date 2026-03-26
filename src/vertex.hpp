#ifndef VERTEX_HPP
#define VERTEX_HPP

class Vertex {
    public:
        double x;
        double y;
        double z;
        Vertex();
        Vertex(double a, double b, double c);
        Vertex mid(const Vertex v);
        double dot(const Vertex v);
        Vertex minVx(const Vertex v);
        Vertex maxVx(const Vertex v);
};
    
Vertex operator-(const Vertex& v1, const Vertex& v2);
Vertex operator+(const Vertex& v1, const Vertex& v2);
Vertex operator*(const Vertex& v1, const Vertex& v2);

#endif