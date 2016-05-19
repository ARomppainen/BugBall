#pragma once

#include <iostream>
#include <vector>
#include <ostream>

#include "Board.h"
#include "Util.h"

#include "IwGeomVec2.h"

typedef CIwVec2 vec2i;

class AStar
{
public:

    static std::vector<vec2i> findPath(vec2i start, vec2i goal, Board& board);

private:

    AStar();
};

class Node
{
public:

    vec2i point;
    int g; // distance from start to this node
    int h; // distance from this node to goal (heuristic)

    std::vector<vec2i> predecessors;

public:

    Node(vec2i point) :
        g(0),
        h(0),
        point(point)
    {

    }

    ~Node()
    {

    }

    int getF() const
    {
        return g + h;
    }

    void setPredecessor(Node n)
    {
        this->predecessors = n.predecessors;
        this->predecessors.push_back(n.point);
    }

    int getHash() const
    {
        std::string s;
        s.push_back(point.x);
        s.push_back(':');
        s.push_back(point.y);
        return Util::getHash(s);
    }

    bool operator<(const Node& rhs) const
    {
        return this->getF() < rhs.getF();
    }

    bool operator>(const Node& rhs) const
    {
        return rhs < *this;
    }

    bool operator==(const Node& rhs) const
    {
        return (this->point.x == rhs.point.x &&
                this->point.y == rhs.point.y);
    }

    friend std::ostream& operator<<(std::ostream& out, const Node& n)
    {
        out << "(" << n.point.x << "," << n.point.y << ")";
        return out;
    }
};