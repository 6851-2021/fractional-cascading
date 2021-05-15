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
        map<T, map <T, BridgeRecord<T>* > > D_uv_bottom;
        map<T, map <T, BridgeRecord<T>* > > D_uv_top;
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
                Record* prevRecord =  nodeCat.getBottomRecord();
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
                    record_to_insert->setUpPointer(prevRecord->getUpPointer());
                    prevRecord->setUpPointer(&record_to_insert);
                    prevRecord = record_to_insert;
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
            queue<AugmentedRecord*> count_queue;
            while (wide_gap_queue.size()) {
                BridgeRecord* b = wide_gap_queue.front();
                BridgeRecord* b_comp = b->getCompanionBridge();
                Edge e = b.getEdge();
                wide_gap_queue.pop();
                // merge gaps
                AugmentedRecord* b_gap = b->getPrevBridge()->getUpPointer();
                AugmentedRecord* b_comp_gap = b_comp->getPrevBridge()->getUpPointer();
                int recordNum = 0;
                AugmentedRecord* lastRecord;
                while(b_gap != b || b_comp_gap != b_comp) {
                    if(b_gap != b) {
                        if(b_comp_gap != b_comp) {
                            if(b_gap->getKey() <= b_comp_gap->getKey()) {
                                lastRecord = b_gap;
                                b_gap = b_gap->getUpPointer();
                            } else {
                                lastRecord = b_comp_gap;
                                b_comp_gap = b_comp_gap->getUpPointer();
                            }
                        } else {
                            lastRecord = b_gap;
                            b_gap = b_gap->getUpPointer();
                        }
                    } else {
                        if(b_comp_gap != b_comp) {
                            lastRecord = b_comp_gap;
                            b_comp_gap = b_comp_gap->getUpPointer();
                        }
                    }
                    recordNum++;
                    //construct new bridges and add to count queue
                    if(recordNum%(3*d) == 0){
                        BridgeRecord b_copy(lastRecord->getKey(), lastRecord->getCPointer(),
                            lastRecord->getFlag(), e);
                        BridgeRecord b_comp_copy(lastRecord->getKey(), lastRecord->getCPointer(),
                            lastRecord->getFlag()), e;
                        b_copy->setUpPointer(b_gap);
                        b_comp_copy->setUpPointer(b_comp_gap);
                        b_copy->setDownPointer(b_gap->getDownPointer());
                        b_comp_copy->setDownPointer(b_comp_gap->getDownPointer());

                        b_gap->getDownPointer()->setUpPointer(b_copy);
                        b_comp_gap->getDownPointer()->setUpPointer(b_comp_copy);
                        b_gap->setDownPointer(b_copy);
                        b_comp_gap->setDownPointer(b_comp_copy);

                        b_copy->setPrevBridge(b->getPrevBridge());
                        b_copy->setCompanionBridge(b_comp_copy);
                        b->setPrevBridge(b_copy);
                        b_comp_copy->setPrevBridge(b_comp->getPrevBridge());
                        b_comp->setPrevBridge(b_comp_copy);
                        b_comp_copy->setCompanionBridge(b_copy);

                        count_queue.push(b_copy);
                        count_queue.push(b_comp_copy);
                    }
                }
            }
            updateCountFields(count_queue);
        }

        void constructAugmentedCatalogs() {
            queue<AugmentedRecord*> count_queue;
            map<T,Node<T>>::iterator it = nodes_.begin();
            while (it != nodes_.end()) {
                T label = it->first;
                Node<T> node = it->second;
                AugmentedRecord* prev = NULL;
                Record* record = node.catalog.getBottomRecord();
                while(record) {
                    //Stage 1: Insert a copy of p (called r in the paper) into A_v and a pointer to r into count_queue
                    AugmentedRecord augRecord;
                    if(record.getEndOfRange()){
                        // companion bridges? also which endpoint?
                        augRecord = BridgeRecord<T>(record->getKey(), record, 0, record->getEdge());
                        if(record->getEdge().getRange().first == record->getKey()) {
                            if(record->getEdge().getEndpoints().first == label) {
                                if(D_uv_bottom[record->getEdge().getEndpoints().second][label]){
                                    augRecord.setCompanionBridge(D_uv_bottom[record->getEdge().getEndpoints().second][label]);
                                    D_uv_bottom[record->getEdge().getEndpoints().second][label]->setCompanionBridge(&augRecord);
                                }
                                D_uv_bottom[label][record->getEdge().getEndpoints().second] = &augRecord;
                            } else {
                                if(D_uv_bottom[record->getEdge().getEndpoints().first][label]){
                                    augRecord.setCompanionBridge(D_uv_bottom[record->getEdge().getEndpoints().first][label]);
                                    D_uv_bottom[record->getEdge().getEndpoints().first][label]->setCompanionBridge(&augRecord);
                                }
                                D_uv_bottom[label][record->getEdge().getEndpoints().first] = &augRecord;
                            }
                        } else {
                            if(record->getEdge().getEndpoints().first == label) {
                                augRecord.setPrevBridge(D_uv_bottom[label][record->getEdge().getEndpoints().second]);
                                if(D_uv_top[record->getEdge().getEndpoints().second][label]){
                                    augRecord.setCompanionBridge(D_uv_top[record->getEdge().getEndpoints().second][label]);
                                    D_uv_top[record->getEdge().getEndpoints().second][label]->setCompanionBridge(&augRecord);
                                }
                                D_uv_top[label][record->getEdge().getEndpoints().second] = &augRecord;
                            } else {
                                augRecord.setPrevBridge(D_uv_bottom[label][record->getEdge().getEndpoints().first]);
                                if(D_uv_top[record->getEdge().getEndpoints().first][label]){
                                    augRecord.setCompanionBridge(D_uv_top[record->getEdge().getEndpoints().first][label]);
                                    D_uv_top[record->getEdge().getEndpoints().first][label]->setCompanionBridge(&augRecord);
                                }
                                D_uv_top[label][record->getEdge().getEndpoints().first] = &augRecord;
                            }
                        }
                        count_queue.push(augRecord);
                    } else {
                        augRecord = AugmentedRecord(record.getKey(), &record, 0);
                        count_queue.push(augRecord);
                    }
                    if(prev) {
                        augRecord.setDownPointer(prev);
                        prev->setUpPointer(&augRecord);
                    } 
                    prev = &augRecord;
                    record = record->getUpPointer();
                }
                updateCountFields(count_queue);
                node.createLookupTable();
            }
        }
        
        // /**
        //  * Given x, a key value, and a generalized path of the graph G, in which every edge contains x
        //  * the query looks up x succesively in the catalogs of each vertex in this path, and reports 
        //  * the first value greater than or equal to x
        //  */
        // list<int> multipleLookUpQuery(int x, list<Edge<T>> path_edges) {
        //     list<int> sigma_x;
        //     Edge<T> first_edge = path_edges[0];
        //     Node<T> f = first_edge.endpoints.first;
        //     AugmentedRecord r = f.search(x); //Get Augmented Record thru Lookup
        //     auto ac_pointer = r.getCPointer();
        //     sigma_x.push_back(ac_pointer*); //Carryover lookup into the catalog

        //     for (Edge<T> edge in path_edges) {
        //         //Find bridge for first node in edge to second node 
        //         Node<T> v = edge.endpoints.first;
        //         Node<T> w = edge.endpoints.second;
        //         AugmentedCatalog* A_v = v.getAugCatalog();
        //         bool bridge_found = false;
        //         while (bridge_found != true) {
        //             if (r.isBridge == true) {
        //                 //Check the edge to see if it (v,w)
        //                 // If so, stop the loop and continue to bridge_found
        //                 bridge_found = true;
        //             }
        //             auto up_pointer = r.getUpPointer();
        //             r = *up_pointer;            
        //         }

        //     }

        //     //Notes for me: For each node u and edge e conn. u w/ v, a list of bridges from u to v
        //     // is defined as an ordered subset of the records in A_v having values common to A_u andA_v and lying in Range R_e
        //     // endpoints of R_e are the first and last records in D_uv
        //     return sigma_x;
        // }
};