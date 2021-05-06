#include "Edge.h"
#include "Node.h"
#include "BridgeRecord.h"

#include <set>
#include <list>
#include <map>
#include <queue>

using namespace std;

template <typename T> 
//T represents the type of the label for the nodes in this graph. Preferrably, T should be an immutable type. 

class CatalogGraph {
    /**
     * AF(nodes_,edges_,d) = A catalog graph with nodes "nodes_" and edges "edges_". 
     *                       d is the locally bounded degree of the graph. nodes_ maps every node's label to a Node object
     *                       and edges_ maps every edges integer label to a 
     *                        
     */
    private:
        map<T,Node<T>> nodes_;
        set<Node<T>> allNodes;
        map<int,Edge<T>> edges_;
        set<Edge<T>> allEdges;
        int d; //leave as a paramter
        map <T,list<BridgeRecord<T>> D_uv;
        map <T,list<BridgeRecord<T>> D_vu;
    public:
        CatalogGraph(map<T, list<Record>> nodes, map<int,list<T>>edges, map<int,int[2]>edge_ranges, int d) {
            map<T, list<Record>>::iterator it = nodes.begin();
            while (it != nodes.end()) {
                T label = it->first;
                list<Record> records = it->second;
                Node<T> nodeObject(label, Catalog(records));
                nodes_.insert(pair<T,Node<T>> (label, nodeObject));
                allNodes.insert(nodeObject);
                it++;
            }
            map<int, list<T>>::iterator it = edges.begin();
            while (it != edges.end()) {
                // Accessing KEY from element pointed by it.
                T edge_key= it->first;
                // Accessing VALUE from element pointed by it.
                list<T> endpoints = it->second;
                //Insert into  map and set.
                Edge<T> edgeObject(endpoints,edge_ranges.at(edge_key))
                edges_.insert(pair<int, Edge<T>> (edge_key, edgeObject));
                allEdges.insert(edgeObject);
                // Increment the Iterator to point to next entry
                it++;
            }
        };

        set<Node> getNodes() {
            return allNodes;
        }

        set<Edge> getEdges() {
            return allEdges;
        }


        void updateCountFields(queue<AugmentedRecord*> count_queue) {
            for (AugmentedRecord* record:count_queue) {
                
                for (int i=0; i<6*d;i++) {
                    curr_record->setFlag(1);
                    curr_record = curr_record->getDownPointer(); 
                    if (curr_record == NULL) {
                        break;
                    }
                }
            }
            processClusters(count_queue);
        }

        void processClusters(queue<AugmentedRecord*> count_queue) {
            queue<BridgeRecord*> wide_gap_queue;
            while(count_queue.size()) {
                AugmentedRecord* curr_record = count_queue.front();
                count_queue.pop();
                if(curr_record->getFlag()) {
                    // find bottom record in cluster
                    AugmentedRecord* p = curr_record;
                    while(p->getDownPointer() && p->getDownPointer()->getFlag()) {
                        p = curr_record->getDownPointer();
                    }
                    // ranking process
                    // how do we keep track of bridges? access bridges?
                    int rank = 1;
                    int pastOnes = 0;
                    while(p->getUpPointer() && (p->getUpPointer()->getFlag() || pastOnes < 6 * d)) {
                        if(p->getBridge()) {
                            p->setRank(rank);
                            int newRecords = p->getRank()-p->getPrevBridge()->getRank();
                            p->setCount(p->getCount() + newRecords);
                            int k = p->getCompanionBridge()->getCount();
                            if(p->getCount() + k >= 6 * d && p->getCount() + k - newRecords < 6 * d) {
                                // need a wide gap queue
                            }
                            p->getPrevBridge()->setRank(0);
                        }
                        rank++;
                        p->setFlag(0);
                        if(!p->getFlag()) pastOnes++;
                    }
                    // p->setRank(0);
                }
            }
            // restore gap invariants

        }

        void constructAugmentedCatalogs() {
            queue<AugmentedRecord*> count_queue;
            map<T,Node<T>>::iterator it = nodes_.begin();
            while (it != nodes_.end()) {
                T label = it->first;
                Node<T> node = it->second;
                list<AugmentedRecord>::iterator lit = node.acatalog.listOfRecords.begin();
                for (Record record:node.catalog.listOfRecords) {
                    //Stage 1: Insert a copy of p (called r in the paper) into A_v and a pointer to r into count_queue
                    count_queue.push(node.acatalog.insert(record, lit));
                    updateCountFields(count_queue);
                }
            }
        }
        /**
         * Given x, a key value, and a generalized path of the graph G, in which every edge contains x
         * the query looks up x succesively in the catalogs of each vertex in this path, and reports 
         * the first value greater than or equal to x
         */
        list<int> multipleLookUpQuery(int x, list<T> path) {
            list<int> x_succesors;
            return x_succesors;
        }

};