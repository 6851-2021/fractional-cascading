#include "HiveGraph.h"

int main () {
    vector<pair< pair<int, int>, pair<int, int> > > rays;
    rays.push_back(make_pair(make_pair(2, 1), make_pair(2, 2)));
    rays.push_back(make_pair(make_pair(1, 3), make_pair(1, 4)));
    rays.push_back(make_pair(make_pair(5, 1), make_pair(5, 4)));
    HiveGraph smallTest(rays, true);
    smallTest.printAdjList();
}