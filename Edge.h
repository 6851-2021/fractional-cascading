#pragma once

#include "Node.h"

#include <set>
#include <list>
#include <utility>

using namespace std;

template <typename T>
class Edge {
    private:
        pair<T,T> endpoints;
        pair<float,float> range;
    public:
        Edge(pair<T,T> ends, pair<float,float> range) {
            this->endpoints = ends;
            this->range = range;
        }

        pair<T, T> getEndpoints(){
            return this->endpoints;
        }

        pair<float, float> getRange() {
            return this->range;
        }

        
};