#include "Edge.h"
#include "Node.h"
#include "BridgeRecord.h"

#include <utility>
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
        map<T,Node<T>* > nodes_;
        set<Node<T>* > allNodes;
        map<int,Edge<T>* > edges_;
        set<Edge<T>* > allEdges;
        int d; 
        map<T, map <T,list<BridgeRecord<T> > > D_uv;
    public:
        CatalogGraph(map<T, list<int> > nodes, map<int,pair<T,T> >edges, map<int,pair<int,int> > edge_ranges, int d) {
            //Step 1: Set d field
            this-> d = d;
            
            //Step 2: Process all edges for fields
            map<T, set<int> > ranges;
            map<T, map<int,pair<int,int> > edges_of_interest;
            //Process edges
            for (auto const& edge_and_endpoints : edges) {
                int edge_label = edge_and_endpoints.first;
                pair<T,T> endpoints = edge_and_endpoints.second;
                pair<int,int> edge_range = edge_ranges[edge_label];

                //extract the endpoints for ranges  dictionary
                T endpoint1 = endpoints.first;
                T endpoint2 = endpoints.second;

                //place in endpoints dictionary
                ranges[endpoint1].insert(edge_range.first);
                ranges[endpoint1].insert(edge_range.second);
                ranges[endpoint2].insert(edge_range.first);
                ranges[endpoint2].insert(edge_range.second);

                //place in edges_of_interest_dictionary. This maps every node to the edges incident on it. 
                //stroign with it the relevant endpoints
                edges_of_interest[endpoint1][edge_label] = make_pair(edge_range.first,edge_range.second);
                edges_of_interest[endpoint2][edge_label] = make_pair(edge_range.first,edge_range.second);

                //Place them in the appropriate fields
                Edge<T>* edgeObject = new Edge(endpoints,edge_ranges);
                edges_[edge_label] = edgeObject;
                allEdges.insert(edgeObject);

            }

            //Step 3: Process all nodes for fields
            for (auto const& key_and_value: nodes){
                //Get label and list of values
                T  label = key_and_value.first;
                list<int> values = key_and_value.second;

                //Create nodes and place in appropriate fields
                Node<T>* nodeObject = new Node(label);
                nodes_[label] = nodeObject;
                allNodes.insert(nodeObject);
                
                //Fill up node's catalog
                Catalog nodeCat = nodeObject->getCatalog();
                set<int> rangeEnds = ranges[label];
                values.sort();
                list<Record>::iterator insertion_iterator =  nodeCat.getIterator();
                list<int>::iterator it = values.begin();
                while (it! = values.end()){
                    int value = *it;
                    Record* record_to_insert;
                    if (rangeEnds.find(key) != rangeEnds.end()) {
                        //Figure out the edge whose endpoint contains `value`
                        for (auto const& edge_and_endpoints : edges_of_interest[label]) {
                                int edge_key =  edge_and_endpoints.first;
                                pair<int,int> rangepoints = edge_and_endpoints.second;

                                int  endpoint1 = rangepoints.first;
                                int  endpoint2 = rangepoints.second;

                                //We reach this case once and after that, never again for any particular node
                                if  (endpoint1 == value && endpoint1 != endpoint2) {
                                    record_to_insert = new Record(value,true,edge_key);
                                    edges_of_interest[label][edge_key] = make_pair(endpoint2,endpoint2); 
                                    break;
                                //Reach this case the next time round and then delete the edge.
                                } else if (endpoint2 == value) {
                                    record_to_insert = new Record(value,true,edge_key);
                                    edges_of_interest[label].erase(edge_key);
                                    break;
                                }
                        }
                        
                    } else {
                        record_to_insert = new Record(value,false,nullptr);
                    }
                    insertion_iterator = nodeCat.insert(record_to_insert,insertion_iterator);
                    it++;
                }

                
            }
        };
        

        set<Node<T>*> getNodes() {
            return allNodes;
        }

        set<Edge<T>*> getEdges() {
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
                    int rank = 1;
                    int pastOnes = 0;
                    BridgeRecord* lastBridge;
                    while(p->getUpPointer() && (p->getUpPointer()->getFlag() || pastOnes < 6 * d)) {
                        if(p->getBridge()) {
                            p->setRank(rank);
                            int newRecords = p->getRank()-p->getPrevBridge()->getRank();
                            p->setCount(p->getCount() + newRecords);
                            int k = p->getCompanionBridge()->getCount();
                            if(p->getCount() + k >= 6 * d && p->getCount() + k - newRecords < 6 * d) {
                                wide_gap_queue.push(p);
                            }
                            p->getPrevBridge()->setRank(0);
                            lastBridge = p;
                        }
                        rank++;
                        p->setFlag(0);
                        if(!p->getFlag()) pastOnes++; 
                        p = p->getUpPointer();
                    }
                    lastBridge->setRank(0);
                }
            }
            restoreGapInvariants(queue<BridgeRecord*> wide_gap_queue);
        }

        void restoreGapInvariants(queue<BridgeRecord*> wide_gap_queue) {
            // gap to restore is betweeen b and b->getPrevBridge()
            while (wide_gap_queue.size()) {
                BridgeRecord* b = wide_gap_queue.front();
                BridgeRecord* b_comp = b->getCompanionBridge();
                wide_gap_queue.pop();
                list<AugmentedRecord*> mergedGap;
                AugmentedRecord* b_gap = b->getPrevBridge()->getUpPointer();
                AugmentedRecord* b_comp_gap = b_comp->getPrevBridge()->getUpPointer();
                //merge gaps into mergedGap
                //split into groups of size 3d
                //construct new bridges and add to count queue
            }
        }

        void constructAugmentedCatalogs() {
            queue<AugmentedRecord*> count_queue;
            map<T,Node<T>>::iterator it = nodes_.begin();
            while (it != nodes_.end()) {
                T label = it->first;
                Node<T> node = it->second;
                list<AugmentedRecord*>::iterator lit = node.acatalog.listOfRecords.begin();
                for (Record record:node.catalog.listOfRecords) {
                    //Stage 1: Insert a copy of p (called r in the paper) into A_v and a pointer to r into count_queue
                    if(record.getEndOfRange()){
                        count_queue.push(node.acatalog.insert(record, lit, true));
                    } else {
                        count_queue.push(node.acatalog.insert(record, lit, false));
                    }
                    updateCountFields(count_queue);
                }
                node.createLookupTable();
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