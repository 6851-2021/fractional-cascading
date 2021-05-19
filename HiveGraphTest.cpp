#include "HiveGraph.h"

int main () {
    vector<pair< pair<int, int>, pair<int, int> > > rays;
    rays.push_back(make_pair(make_pair(1, 2), make_pair(1, 9)));
    rays.push_back(make_pair(make_pair(3, 3), make_pair(3, 4)));
    rays.push_back(make_pair(make_pair(4, 5), make_pair(4, 7)));
    rays.push_back(make_pair(make_pair(6, 2), make_pair(6, 7)));
    rays.push_back(make_pair(make_pair(7, 3), make_pair(7, 6)));
    rays.push_back(make_pair(make_pair(8, 1), make_pair(8, 5)));
    rays.push_back(make_pair(make_pair(8, 7), make_pair(8, 9)));
    HiveGraph smallTest(rays, true);
    smallTest.printAdjList();
    smallTest.visualizeConstruction();
}