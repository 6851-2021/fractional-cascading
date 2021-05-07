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
        /**
         * Given x, a key value, and a generalized path of the graph G, in which every edge contains x
         * the query looks up x succesively in the catalogs of each vertex in this path, and reports 
         * the first value greater than or equal to x
         */
        list<int> multipleLookUpQuery(int x, list<Node> path_nodes, list<Edge> path_edges) {
            list<int> sigma_x;
            list<int> positions;
            set<Node> nodes_known;
            set<Edge> edges_checked;

            auto f = path_nodes[0]; //or path_edges[0][0] depending on implementation
            auto r = f.acatalog.search(x); //This and below assumes acatalog is a reg. list
            sigma_x.add(f.acatalog.at(r));
            positions.add(r);

            for (Edge edge in path_edges) {
                if (edge not in edges_checked) {
                    if(edge[0] in nodes_known) {
                        Node v = edge[0];
                        Node w = edge[1];
                        AugmentedCatalog A_v = v.acatalog;
                        //Go thru catalog for bridge
                        //Check if edge for bridge is (v,w)
                        //bridgefound(A_w,i,x)
                    }
                    else if(edge[1] in nodes_known) {
                        Node v = edge[1];
                        Node w = edge[0];
                        AugmentedCatalog A_v = v.acatalog;
                        //Go thru catalog for bridge
                        //Check if edge for bridge is (v,w)
                        //bridgefound(A_w,i,x)
                    } 
                }
            }

            //Notes for me: For each node u and edge e conn. u w/ v, a list of bridges from u to v
            // is defined as an ordered subset of the records in A_v having values common to A_u andA_v and lying in Range R_e
            // endpoints of R_e are the first and last records in D_uv
            return sigma_x;
        }

        int bridgeFound(BridgeRecord A_w, int i, int x) {
            x_found = false; 
            //curr = A_w at i
            prev = 0;
            while (x_found == false){
                //Find x thru pointers
                x_found = true;
            }
        return prev; //curr
        }

        //Given a list of ints arr, return int at i
        T get<T>(list<T> arr, int i) {
            auto list_pointer = arr.begin();
            std::advance(arr,i);
            return *list_pointer;
        }

    

};