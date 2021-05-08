#include "Node.h"

#include <set>
#include <list>
#include <utility>

using namespace std;

template <typename T>
class Edge {
    private:
        pair<T,T> endpoints;
        pair<int,int> range;
    public:
        Edge(pair<T,T> ends, pair<int,int> range) {
            this->endpoints = ends;
            this->range = range;
        }

        
};