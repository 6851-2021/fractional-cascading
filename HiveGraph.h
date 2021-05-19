#include <utility>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>

using namespace std;

int inf = numeric_limits<int>::max();
int neg_inf = -1*inf;

bool comparatorY (pair<pair<int, int>, int > point1, pair<pair<int, int>, int > point2) {
    return point1.first.first < point2.first.first;
}

bool comparatorXray (pair<pair<int, int>, pair<int, int> > point1, pair<pair<int, int>, pair<int, int> > point2) {
    return point1.first.first < point2.first.first;
}

bool comparatorX (pair<int, int> ray1, pair<int, int> ray2) {
    return ray1.second < ray2.second;
}

bool compair (pair<int, int> ray1, pair<int, int> ray2) {
    return ray1.first < ray2.first;
}

class HiveGraph {
    private:
        map<pair<int, int>, set<pair<int, int> > > adjList;      
        map<int, map<pair< pair<int, int>, pair<int, int> >, int > > segAdj;

    public:
        HiveGraph(vector<pair<pair<int, int>, pair<int, int> > > rays, bool vertical) {
            ofstream fout;
            fout.open("points.txt");
            vector<pair< pair<int, int> , int> > endpoints;
            for(int i = 0; i < rays.size(); i++) {
                if(!vertical){
                    rays[i] = make_pair(make_pair(rays[i].first.second, rays[i].first.first), 
                                            make_pair(rays[i].second.second, rays[i].second.first));
                }
            }
            sort(rays.begin(), rays.end(), comparatorXray);
            for(int i = 0; i < rays.size(); i++) {
                fout << "(" << rays[i].first.first << "," << rays[i].first.second << "," << rays[i].second.first << "," << rays[i].second.second << ") ";
                endpoints.push_back(make_pair(make_pair(rays[i].first.second, rays[i].first.first), i));
                endpoints.push_back(make_pair(make_pair(rays[i].second.second, rays[i].second.first), i));
            }
            fout << endl;
            sort(endpoints.begin(), endpoints.end(), comparatorY);
            map<int, int > maxY;
            set<pair<int, int> > active;
            // create longest horizontal segments not interesecting given vertical segments
            for(int i = 0; i < 2*rays.size(); i++) {
                pair<int, int> x = make_pair(endpoints[i].first.second, endpoints[i].second);
                auto up = active.upper_bound(x);
                auto down = active.lower_bound(x);
                if(down == active.begin()) {
                    down = active.end();
                } else {
                    down = prev(down);
                }
                pair<int, int> fixed_endpoint = make_pair(endpoints[i].first.second, endpoints[i].first.first);
                if(up != active.end()){
                    adjList[fixed_endpoint].insert(make_pair(rays[up->second].first.first, endpoints[i].first.first));
                    adjList[make_pair(rays[up->second].first.first, endpoints[i].first.first)].insert(fixed_endpoint);
                    segAdj[endpoints[i].second][make_pair(fixed_endpoint, make_pair(rays[up->second].first.first, endpoints[i].first.first))] = up->second;
                    segAdj[up->second][make_pair(make_pair(rays[up->second].first.first, endpoints[i].first.first), fixed_endpoint)] = endpoints[i].second;
                    adjList[make_pair(rays[up->second].first.first, maxY[rays[up->second].first.first])].insert(make_pair(rays[up->second].first.first, endpoints[i].first.first));
                    adjList[make_pair(rays[up->second].first.first, endpoints[i].first.first)].insert(make_pair(rays[up->second].first.first, maxY[rays[up->second].first.first]));
                    maxY[rays[up->second].first.first] = endpoints[i].first.first;
                } else {
                    adjList[fixed_endpoint].insert(make_pair(inf, endpoints[i].first.first));
                    segAdj[endpoints[i].second][make_pair(fixed_endpoint, make_pair(inf, endpoints[i].first.first))] = -1;
                    segAdj[-1][make_pair(make_pair(inf, endpoints[i].first.first), fixed_endpoint)] = endpoints[i].second;
                }
                if(down != active.end()) {
                    adjList[fixed_endpoint].insert(make_pair(rays[down->second].first.first, endpoints[i].first.first));
                    adjList[make_pair(rays[down->second].first.first, endpoints[i].first.first)].insert(fixed_endpoint);
                    segAdj[endpoints[i].second][make_pair(fixed_endpoint, make_pair(rays[down->second].first.first, endpoints[i].first.first))] = down->second;
                    segAdj[down->second][make_pair(make_pair(rays[down->second].first.first, endpoints[i].first.first), fixed_endpoint)] = endpoints[i].second;
                    adjList[make_pair(rays[down->second].first.first, maxY[rays[down->second].first.first])].insert(make_pair(rays[down->second].first.first, endpoints[i].first.first));
                    adjList[make_pair(rays[down->second].first.first, endpoints[i].first.first)].insert(make_pair(rays[down->second].first.first, maxY[rays[down->second].first.first]));
                    maxY[rays[down->second].first.first] = endpoints[i].first.first;
                } else {
                    adjList[fixed_endpoint].insert(make_pair(neg_inf, endpoints[i].first.first));
                    segAdj[endpoints[i].second][make_pair(fixed_endpoint, make_pair(neg_inf, endpoints[i].first.first))] = -2;
                    segAdj[-2][make_pair(make_pair(neg_inf, endpoints[i].first.first), fixed_endpoint)] = endpoints[i].second;
                }
                if(rays[endpoints[i].second].second.second == endpoints[i].first.first) {
                    if(maxY[x.first] == rays[x.second].first.second) {
                        adjList[rays[x.second].first].insert(rays[x.second].second);
                        adjList[rays[x.second].second].insert(rays[x.second].first);
                    }
                    active.erase(x);
                } else {
                    active.insert(x);
                    maxY[x.first] = endpoints[i].first.first;
                }
            }
            // remove out-of-date/bad neighbors
            for(auto it = adjList.begin(); it != adjList.end(); it++) {
                cout << it->first.first << " " << it->first.second << " ";
                pair<int, int> right;
                pair<int, int> left;
                pair<int, int> up;
                pair<int, int> down;
                set<pair<int, int> > remove;
                bool seenRight = false, seenLeft = false, seenUp = false, seenDown = false;
                for(auto set_it = it->second.begin(); set_it != it->second.end(); set_it++) {
                    if(set_it->first < it->first.first) {
                        if(!seenLeft) {
                            seenLeft = true;
                            left = *set_it;
                        } else {
                            if(left.first <= set_it->first) {
                                remove.insert(left);
                                left = *set_it;
                            } else {
                                remove.insert(*set_it);
                            }
                        }
                    } else if(set_it->first > it->first.first) {
                        if(!seenRight) {
                            seenRight = true;
                            right = *set_it;
                        } else {
                            if(right.first >= set_it->first) {
                                remove.insert(right);
                                right = *set_it;
                            } else {
                                remove.insert(*set_it);
                            }
                        }
                    } else if(set_it->second > it->first.second) {
                        if(!seenUp) {
                            seenUp = true;
                            up = *set_it;
                        } else {
                            if(up.second >= set_it->second) {
                                remove.insert(up);
                                up = *set_it;
                            } else {
                                remove.insert(*set_it);
                            }
                        }
                    } else if(set_it->second < it->first.second) {
                        if(!seenDown) {
                            seenDown = true;
                            down = *set_it;
                        } else {
                            if(down.second <= set_it->second) {
                                remove.insert(down);
                                down = *set_it;
                            } else {
                                remove.insert(*set_it);
                            }
                        }
                    } else {
                        remove.insert(*set_it);
                    }

                }
                for(auto remove_it = remove.begin(); remove_it != remove.end(); remove_it++) {
                    it->second.erase(*remove_it);
                }
                cout << endl;
            }

            for(int i = 0; i < 2*rays.size(); i++) {
                for(auto set_it = adjList[make_pair(endpoints[i].first.second, endpoints[i].first.first)].begin(); set_it != adjList[make_pair(endpoints[i].first.second, endpoints[i].first.first)].end(); set_it++) {
                    if(endpoints[i].first.second != set_it->first){
                        fout << "(" << endpoints[i].first.second << "," << endpoints[i].first.first << "," << set_it->first << "," << set_it->second << ") ";
                
                    }
                }
            }
            fout << endl;
            // clean up segAdj
            for(int i = 0; i < 2*rays.size(); i++) {
                set< pair<pair<int, int>, pair<int, int> > > removeSet;
                for(auto it = segAdj[endpoints[i].second].begin(); it != segAdj[endpoints[i].second].end(); it++) {
                    if(adjList.count(it->first.first)) {
                        if(!adjList[it->first.first].count(it->first.second)) {
                            removeSet.insert(it->first);
                       }
                   } else if(adjList.count(it->first.second)) {
                        if(!adjList[it->first.second].count(it->first.first)) {
                           removeSet.insert(it->first);
                        }
                    } else {
                        removeSet.insert(it->first);
                    }
                }
                for(auto it = removeSet.begin(); it != removeSet.end(); it++) {
                    if(segAdj[endpoints[i].second].count(*it)) {
                        segAdj[endpoints[i].second].erase(*it);      
                    }
                }
            }
        
            // sweep from right to left to remove anomalies
            for(int i = rays.size()-1; i >= 0; i--) {
                set< pair< pair<pair<int, int>, pair<int, int> >, pair<int, int> > > pushList;
                vector<pair<pair<pair<int, int>, pair<int, int> >, int> > belows;
                for(auto it = segAdj[i].begin(); it != segAdj[i].end(); it++) {
                    if(it->first.second.first <= it->first.first.first) {
                        belows.push_back(*it);
                    }
                }
                if(belows.size() == 0){
                    continue;
                } else {
                    int parity = 1;
                    int j = 0;
                    bool firstBelow = false;
                    for(auto it = segAdj[i].begin(); it != segAdj[i].end(); it++) {
                        if(it->first.first.second > belows[j].first.first.second && it->first.first.second < belows[j+1].first.first.second) {
                            if(parity%2 != 0) {
                                // push above segment to below
                                //adjList[it->first.first].insert(make_pair(belows[j].first.second.first, it->first.first.second));
                                //adjList[make_pair(belows[j].first.second.first, it->first.first.second)].insert(it->first.first);
                                pushList.insert(make_pair(make_pair(it->first.first, make_pair(belows[j].first.second.first, it->first.first.second)), make_pair(i, belows[j].second)));
                                fout << "(" << it->first.first.first << "," << it->first.first.second << "," << belows[j].first.second.first << "," << it->first.first.second << ") ";
                            }
                            parity++;
                        } else if(it->first.second.first <= it->first.first.first) {
                            if(firstBelow){
                                if(j == belows.size()-2){
                                    break;
                                }
                                j++;
                            } else {
                                firstBelow = true;
                            }
                            parity = 1;
                        }
                    }
                    for(auto it = pushList.begin(); it != pushList.end(); it++) {
                        segAdj[it->second.first][it->first] = it->second.second;
                        segAdj[it->second.second][make_pair(it->first.second, it->first.first)] = it->second.first;
                    }
                }
            }
            
            // sweep from left to right to remove anomalies
            for(int i = 0; i < rays.size(); i++) {
                set< pair< pair<pair<int, int>, pair<int, int> >, pair<int, int> > > pushList;
                vector<pair<pair<pair<int, int>, pair<int, int> >, int> > aboves;
                for(auto it = segAdj[i].begin(); it != segAdj[i].end(); it++) {
                    if(it->first.second.first >= it->first.first.first) {
                        aboves.push_back(*it);
                    }
                }
                if(aboves.size() == 0){
                    continue;
                } else {
                    int parity = 1;
                    int j = 0;
                    bool firstAbove = false;
                    for(auto it = segAdj[i].begin(); it != segAdj[i].end(); it++) {
                        if(it->first.first.second > aboves[j].first.first.second && it->first.first.second < aboves[j+1].first.first.second) {
                            if(parity%2 != 0) {
                                // push below segment to above
                                //adjList[it->first.first].insert(make_pair(aboves[j].first.second.first, it->first.first.second));
                                //adjList[make_pair(aboves[j].first.second.first, it->first.first.second)].insert(it->first.first);
                                pushList.insert(make_pair(make_pair(it->first.first, make_pair(aboves[j].first.second.first, it->first.first.second)), make_pair(i, aboves[j].second)));
                                fout << "(" << it->first.first.first << "," << it->first.first.second << "," << aboves[j].first.second.first << "," << it->first.first.second << ") ";
                            }
                            parity++;
                        } else if(it->first.second.first >= it->first.first.first) {
                            if(firstAbove){
                                if(j == aboves.size()-2){
                                    break;
                                }
                                j++;
                            } else {
                                firstAbove = true;
                            }
                            parity = 1;
                        }
                    }
                    for(auto it = pushList.begin(); it != pushList.end(); it++) {
                        segAdj[it->second.first][it->first] = it->second.second;
                        segAdj[it->second.second][make_pair(it->first.second, it->first.first)] = it->second.first;
                    }
                }
                
            }
            
        }

        
        void printAdjList() {
            for(auto it = adjList.begin(); it != adjList.end(); ++it)
            {
                for(auto set_it = it->second.begin(); set_it != it->second.end(); set_it++) {
                    cout << it->first.first << ", " << it->first.second << " " << 
                    set_it->first << ", " << set_it->second << endl;
                }
            }
        }

        void visualizeConstruction() {
            system("python3 HiveGraphVisualization.py");
        }
};