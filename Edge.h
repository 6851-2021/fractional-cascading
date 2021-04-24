#include "Node.h"

#include <set>
#include <list>

using namespace std;

template <typename T>
class Edge {
    private:
        list<T> endpoints;
        int range [2];
    public:
        Edge(list<T> ends, int[2] range) {
            this->endpoints = ends;
            this->range = range;
        }

        
};