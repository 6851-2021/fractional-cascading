#include <iostream>
#include "CatalogGraph.h"

int main () {
    map<int, list<float> > nodes;
    list<float> node0;
    node0.push_back(1);
    node0.push_back(3);
    node0.push_back(4);
    node0.push_back(5);
    node0.push_back(6);
    node0.push_back(7);
    node0.push_back(8);
    node0.push_back(9);
    node0.push_back(10);
    nodes[0] = node0;
    list<float> node1;
    node1.push_back(1);
    node1.push_back(2);
    node1.push_back(3);
    node1.push_back(4);
    node1.push_back(5);
    node1.push_back(6);
    node1.push_back(7);
    node1.push_back(8);
    node1.push_back(9);
    node1.push_back(10);
    nodes[1] = node1;
    list<float> node2;
    node2.push_back(2);
    node2.push_back(3);
    node2.push_back(3);
    node2.push_back(4);
    node2.push_back(5);
    nodes[2] = node2;
    map<int,pair<int,int> >edges;
    edges[0] = make_pair(0, 1);
    edges[1] = make_pair(1, 2);
    edges[2] = make_pair(2, 0);
    map<int,pair<float,float> > edge_ranges;
    edge_ranges[0] = make_pair(neg_inf, 10);
    edge_ranges[1] = make_pair(3, 4);
    edge_ranges[2] = make_pair(3, 5);
    int d = 2;
    CatalogGraph<int> catGraph(nodes, edges, edge_ranges, d);
    catGraph.constructAugmentedCatalogs();

    for(Node<int>* n: catGraph.getNodes()){
        cout << n->getLabel() << endl;
        AugmentedRecord* a = n->getAugCatalog()->getBottomRecord();
        while(a) {
            if(a->getBridge()){
                cout << a->getKey() << " ";
            }      
            a = a->getUpPointer();
        }
        cout << endl;
    }

   //Testing multiple lookup query

   cout << "Query testing" << endl;
   list<int> path_edges;
   path_edges.push_back(0);
   path_edges.push_back(1);
   path_edges.push_back(2);
   list<float> ans;
   ans = catGraph.multipleLookUpQuery(3.0,path_edges);
   for (float n: ans) {
       cout << n << endl;
   }
    
};