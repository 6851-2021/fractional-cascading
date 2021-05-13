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
            map<T, map<int,pair<int,int> > > edges_of_interest;
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
                Edge<T>* edgeObject = new Edge<T>(endpoints,edge_range);
                edges_[edge_label] = edgeObject;
                allEdges.insert(edgeObject);

            }

            //Step 3: Process all nodes for fields
            for (auto const& key_and_value: nodes){
                //Get label and list of values
                T  label = key_and_value.first;
                list<int> values = key_and_value.second;

                //Create nodes and place in appropriate fields
                Node<T>* nodeObject = new Node<T>(label);
                nodes_[label] = nodeObject;
                allNodes.insert(nodeObject);
                
                //Fill up node's catalog
                Catalog* nodeCat = nodeObject->getCatalog();
                set<int> rangeEnds = ranges[label];
                values.sort();
                Record* prevRecord =  nodeCat->getBottomRecord();
                list<int>::iterator it = values.begin();
                while (it != values.end()){
                    int value = *it;
                    Record* record_to_insert;
                    if (rangeEnds.find(value) != rangeEnds.end()) {
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
                        record_to_insert = new Record(value,false);
                    }
                    record_to_insert->setUpPointer(prevRecord->getUpPointer());
                    prevRecord->setUpPointer(record_to_insert);
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


        void updateCountFields(deque<AugmentedRecord*> count_queue) {
            for (AugmentedRecord* curr_record:count_queue) {
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

        void processClusters(deque<AugmentedRecord*> count_queue) {
            deque<BridgeRecord<T>*> wide_gap_queue;
            while(count_queue.size()) {
                AugmentedRecord* curr_record = count_queue.front();
                count_queue.pop_front();
                if(curr_record->getFlag()) {
                    // find bottom record in cluster
                    AugmentedRecord* p = curr_record;
                    while(p->getDownPointer() && p->getDownPointer()->getFlag()) {
                        p = curr_record->getDownPointer();
                    }
                    // ranking process
                    int rank = 1;
                    int pastOnes = 0;
                    BridgeRecord<T>* lastBridge;
                    while(p->getUpPointer() && (p->getUpPointer()->getFlag() || pastOnes < 6 * d)) {
                        if(p->getBridge()) {
                            BridgeRecord<T>* b = static_cast<BridgeRecord<T>*>(p);
                            b->setRank(rank);
                            int newRecords = b->getRank()-b->getPrevBridge()->getRank();
                            b->setCount(b->getCount() + newRecords);
                            int k = b->getCompanionBridge()->getCount();
                            if(b->getCount() + k >= 6 * d && b->getCount() + k - newRecords < 6 * d) {
                                wide_gap_queue.push_back(b);
                            }
                            b->getPrevBridge()->setRank(0);
                            lastBridge = b;
                        }
                        rank++;
                        p->setFlag(0);
                        if(!p->getFlag()) pastOnes++; 
                        p = p->getUpPointer();
                    }
                    lastBridge->setRank(0);
                }
            }
            restoreGapInvariants(wide_gap_queue);
        }

        void restoreGapInvariants(deque<BridgeRecord<T>*> wide_gap_queue) {
            // gap to restore is betweeen b and b->getPrevBridge()
            deque<AugmentedRecord*> count_queue;
            while (wide_gap_queue.size()) {
                BridgeRecord<T>* b = wide_gap_queue.front();
                BridgeRecord<T>* b_comp = b->getCompanionBridge();
                Edge<T>* e = b->getEdge();
                wide_gap_queue.pop_front();
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
                        BridgeRecord<T>* b_copy = new BridgeRecord<T>(lastRecord->getKey(), lastRecord->getCPointer(),
                            lastRecord->getFlag(), e);
                        BridgeRecord<T>* b_comp_copy = new BridgeRecord<T>(lastRecord->getKey(), lastRecord->getCPointer(),
                            lastRecord->getFlag(), e);
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

                        count_queue.push_back(b_copy);
                        count_queue.push_back(b_comp_copy);
                    }
                }
            }
            updateCountFields(count_queue);
        }

        void constructAugmentedCatalogs() {
            deque<AugmentedRecord*> count_queue;
            typename map<T,Node<T>* >::iterator it = nodes_.begin();
            while (it != nodes_.end()) {
                T label = it->first;
                Node<T>* node = it->second;
                AugmentedRecord* prev = NULL;
                Record* record = node->getCatalog()->getBottomRecord();
                while(record) {
                    //Stage 1: Insert a copy of p (called r in the paper) into A_v and a pointer to r into count_queue
                    if(record->getEndOfRange()){
                        // companion bridges? also which endpoint?
                        Edge<T>* e = edges_[record->getEdge()];
                        BridgeRecord<T>* augRecord = new BridgeRecord<T>(record->getKey(), record, 0, e);
                        if(e->getRange().first == record->getKey()) {
                            if(e->getEndpoints().first == label) {
                                if(D_uv_bottom[e->getEndpoints().second][label]){
                                    augRecord->setCompanionBridge(D_uv_bottom[e->getEndpoints().second][label]);
                                    D_uv_bottom[e->getEndpoints().second][label]->setCompanionBridge(augRecord);
                                }
                                D_uv_bottom[label][e->getEndpoints().second] = augRecord;
                            } else {
                                if(D_uv_bottom[e->getEndpoints().first][label]){
                                    augRecord->setCompanionBridge(D_uv_bottom[e->getEndpoints().first][label]);
                                    D_uv_bottom[e->getEndpoints().first][label]->setCompanionBridge(augRecord);
                                }
                                D_uv_bottom[label][e->getEndpoints().first] = augRecord;
                            }
                        } else {
                            if(e->getEndpoints().first == label) {
                                augRecord->setPrevBridge(D_uv_bottom[label][e->getEndpoints().second]);
                                if(D_uv_top[e->getEndpoints().second][label]){
                                    augRecord->setCompanionBridge(D_uv_top[e->getEndpoints().second][label]);
                                    D_uv_top[e->getEndpoints().second][label]->setCompanionBridge(augRecord);
                                }
                                D_uv_top[label][e->getEndpoints().second] = augRecord;
                            } else {
                                augRecord->setPrevBridge(D_uv_bottom[label][e->getEndpoints().first]);
                                if(D_uv_top[e->getEndpoints().first][label]){
                                    augRecord->setCompanionBridge(D_uv_top[e->getEndpoints().first][label]);
                                    D_uv_top[e->getEndpoints().first][label]->setCompanionBridge(augRecord);
                                }
                                D_uv_top[label][e->getEndpoints().first] = augRecord;
                            }
                        }
                        count_queue.push_back(augRecord);
                        if(prev) {
                            augRecord->setDownPointer(prev);
                            prev->setUpPointer(augRecord);
                        } 
                        prev = augRecord;
                        record = record->getUpPointer();
                    } else {
                        AugmentedRecord* augRecord = new AugmentedRecord(record->getKey(), record, 0);
                        count_queue.push_back(augRecord);
                        if(prev) {
                            augRecord->setDownPointer(prev);
                            prev->setUpPointer(augRecord);
                        } 
                        prev = augRecord;
                        record = record->getUpPointer();
                    }
                }
                updateCountFields(count_queue);
                node->createLookupTable();
            }
        }
        
        
        /**
         * Given x, a key value, and a generalized path of the graph G, in which every edge contains x
         * the query looks up x succesively in the catalogs of each vertex in this path, and reports 
         * the first value greater than or equal to x
         */

        /*
        list<int> multipleLookUpQuery(int x, list<Node> path_nodes, list<Edge> path_edges) {

            list<int> sigma_x;
            list<int> positions;
            Edge<T> first_edge = path_edges[0];
            Node<T> f = first_edge.endpoints.first;
            AugmentedRecord r = f.acatalog.search(x); //Get Augmented Record thru Lookup
            //sigma_x.add(r.getKey());
            auto ac_pointer = r.getCPointer();
            sigma_x.push_back(ac_pointer*); //Carryover lookup into the catalog

            for (Edge<T> edge in path_edges) {
                //Find bridge for first node in edge to second node 
                Node<T> v = edge.endpoints.first;
                Node<T> w = edge.endpoints.second;
                AugmentedCatalog A_v = v.acatalog;
                //Go thru catalog for bridge
                //Check if edge for bridge is (v,w)
                //bridgefound(A_w,i,x)

            }

            //Notes for me: For each node u and edge e conn. u w/ v, a list of bridges from u to v
            // is defined as an ordered subset of the records in A_v having values common to A_u andA_v and lying in Range R_e
            // endpoints of R_e are the first and last records in D_uv
            return sigma_x;
        }
        

        int bridgeFound(BridgeRecord<T> A_w, int i, int x) {
            x_found = false; 
            //curr = A_w at i
            prev = 0;
            while (x_found == false){
                //Find x thru pointers
                x_found = true;
            }
        return prev; //curr
        }
        */

        //Given a list of ints arr, return int at i
        /*
        T get<T>(list<T> arr, int i) {
            auto list_pointer = arr.begin();
            std::advance(arr,i);
            return *list_pointer;
        }
        */

    

};