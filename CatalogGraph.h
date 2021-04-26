#include "Edge.h"
#include "Node.h"
#include "BridgeRecord.h"

#include <set>
#include <list>
#include <map>

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


        updateCountFields(set<AugmentedRecord*> count_queue) {
            for (AugmentedRecord* record:count_queue) {
                AugmentedRecord* curr_record = record;
                for (int i=0; i<6*d;i++) {
                    curr_record->setFlag(1);
                    curr_record = curr_record->getUpPointer(); 
                    if (curr_record == NULL) {
                        break;
                    }
                }
            }

        }

        void constructAugmentedCaltalogs() {
            set<AugmentedRecord*> count_queue;
            map<T,Node<T>>::iterator it = nodes_.begin();
            while (it != nodes_.end()) {
                T label = it->first;
                Node<T> node = it->second;
                for (Record record:node.catalog.listOfRecords) {
                    //Stage 1: Insert a copy of p (called r in the paper) into A_v and a pointer to r into count_queue
                    count_queue.insert(node.acatalog.insert(record,d));
                    updateCountFields(count_queue);
                }
            }
        }

        list<int> multipleLookUpQuery(int x, list<T> path) {
            
        }

};