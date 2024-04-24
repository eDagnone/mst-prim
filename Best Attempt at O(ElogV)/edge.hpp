#include <string>
#include <iostream>
#include <cstdio>
#pragma once


class Edge {
public:
    Edge* next;
    Edge* parent;
    int destination;
    double weight;
    Edge(int dest, double w, Edge* nextNode) {
        destination = dest;
        weight = w;
        next = nextNode;
        if(nextNode != nullptr)
            nextNode->parent = this;
        parent = nullptr;
    }
    ~Edge() {
        next = nullptr;
        parent = nullptr;
    }
};




