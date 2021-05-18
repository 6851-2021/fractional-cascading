#include <utility>
#include <vector>
#include <map>
#include <set>
#include <iostream>

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

class HiveGraph {
    private:
        map<pair<int, int>, vector<pair<int, int> > > adjList;      
        map<int, map<pair< pair<int, int>, pair<int, int> >, int > > segAdj;

    public:
        HiveGraph(vector<pair<pair<int, int>, pair<int, int> > > rays, bool vertical) {
            vector<pair< pair<int, int> , int> > endpoints;
            for(int i = 0; i < rays.size(); i++) {
                if(!vertical){
                    rays[i] = make_pair(make_pair(rays[i].first.second, rays[i].first.first), 
                                            make_pair(rays[i].second.second, rays[i].second.first));
                }
            }
            sort(rays.begin(), rays.end(), comparatorXray);
            for(int i = 0; i < rays.size(); i++) {
                endpoints.push_back(make_pair(make_pair(rays[i].first.second, rays[i].first.first), i));
                endpoints.push_back(make_pair(make_pair(rays[i].second.second, rays[i].second.first), i));
            }
            sort(endpoints.begin(), endpoints.end(), comparatorY);
            map<int, int > maxY;
            set<pair<int, int> > active;
            // create longest horizontal segments not interesecting given vertical segments
            for(int i = 0; i < 2*rays.size(); i++) {
                pair<int, int> x = make_pair(endpoints[i].first.second, endpoints[i].second);
                set<pair<int, int> >::iterator up = active.upper_bound(x);
                set<pair<int, int> >::iterator down = active.lower_bound(x);
                if(down != active.end() && down != active.begin() && down->first == x.first) {
                    down--;
                } else if(down == active.begin() && down->first >= x.first) {
                    down = active.end();
                }
                pair<int, int> fixed_endpoint = make_pair(endpoints[i].first.second, endpoints[i].first.first);
                if(up != active.end()){
                    adjList[fixed_endpoint].push_back(make_pair(rays[up->second].first.first, endpoints[i].first.first));
                    adjList[make_pair(rays[up->second].first.first, endpoints[i].first.first)].push_back(fixed_endpoint);
                    segAdj[endpoints[i].second][make_pair(fixed_endpoint, make_pair(rays[up->second].first.first, endpoints[i].first.first))] = up->second;
                    segAdj[up->second][make_pair(make_pair(rays[up->second].first.first, endpoints[i].first.first), fixed_endpoint)] = endpoints[i].second;
                    adjList[make_pair(rays[up->second].first.first, maxY[rays[up->second].first.first])].push_back(make_pair(rays[up->second].first.first, endpoints[i].first.first));
                    adjList[make_pair(rays[up->second].first.first, endpoints[i].first.first)].push_back(make_pair(rays[up->second].first.first, maxY[rays[up->second].first.first]));
                    maxY[rays[up->second].first.first] = endpoints[i].first.first;
                } else {
                    adjList[fixed_endpoint].push_back(make_pair(inf, endpoints[i].first.first));
                    segAdj[endpoints[i].second][make_pair(fixed_endpoint, make_pair(inf, endpoints[i].first.first))] = -1;
                    segAdj[-1][make_pair(make_pair(inf, endpoints[i].first.first), fixed_endpoint)] = endpoints[i].second;
                }
                if(down != active.end()) {
                    adjList[fixed_endpoint].push_back(make_pair(rays[down->second].first.first, endpoints[i].first.first));
                    adjList[make_pair(rays[down->second].first.first, endpoints[i].first.first)].push_back(fixed_endpoint);
                    segAdj[endpoints[i].second][make_pair(fixed_endpoint, make_pair(rays[down->second].first.first, endpoints[i].first.first))] = down->second;
                    segAdj[down->second][make_pair(make_pair(rays[down->second].first.first, endpoints[i].first.first), fixed_endpoint)] = endpoints[i].second;
                    adjList[make_pair(rays[down->second].first.first, maxY[rays[down->second].first.first])].push_back(make_pair(rays[down->second].first.first, endpoints[i].first.first));
                    adjList[make_pair(rays[down->second].first.first, endpoints[i].first.first)].push_back(make_pair(rays[down->second].first.first, maxY[rays[down->second].first.first]));
                    maxY[rays[down->second].first.first] = endpoints[i].first.first;
                } else {
                    adjList[fixed_endpoint].push_back(make_pair(neg_inf, endpoints[i].first.first));
                    segAdj[endpoints[i].second][make_pair(fixed_endpoint, make_pair(neg_inf, endpoints[i].first.first))] = -2;
                    segAdj[-2][make_pair(make_pair(neg_inf, endpoints[i].first.first), fixed_endpoint)] = endpoints[i].second;
                }
                if(rays[endpoints[i].second].second.second == endpoints[i].first.first) {
                    if(maxY[x.first] == rays[x.second].first.second) {
                        adjList[rays[x.second].first].push_back(rays[x.second].second);
                        adjList[rays[x.second].second].push_back(rays[x.second].first);
                    }
                    active.erase(x);
                } else {
                    active.insert(x);
                    maxY[x.first] = endpoints[i].first.first;
                }
            }
            // sweep from right to left to remove anomalies
            set< pair< pair<int, int>, pair<int, int> > > pushList;
            for(int i = rays.size()-1; i >= 0; i--) {
                int parity = 1;
                bool firstBelow = false;
                pair<pair<pair<int, int>, pair<int, int> >, int> prevBelow;
                for(auto it = segAdj[i].begin(); it != segAdj[i].end(); it++) {
                    if(firstBelow && it->first.second.first > it->first.first.first) {
                        if(parity%2 != 0) {
                            // push above segment to below
                            adjList[it->first.first].push_back(make_pair(prevBelow.first.second.first, it->first.first.second));
                            adjList[make_pair(prevBelow.first.second.first, it->first.first.second)].push_back(it->first.first);
                            segAdj[i][make_pair(it->first.first, make_pair(prevBelow.first.second.first, it->first.first.second))] = prevBelow.second;
                            segAdj[prevBelow.second][make_pair(make_pair(prevBelow.first.second.first, it->first.first.second), it->first.first)] = i;
                        }
                        parity++;
                    } else if(it->first.second.first <= it->first.first.first) {
                        firstBelow = true;
                        prevBelow = *it;
                        parity = 1;
                    }
                }
            }
            // sweep from left to right to remove anomalies
            pushList.clear();
            for(int i = 0; i < rays.size(); i++) {
                int parity = 1;
                bool firstAbove = false;
                pair<pair<pair<int, int>, pair<int, int> >, int> prevAbove;
                for(auto it = segAdj[i].begin(); it != segAdj[i].end(); it++) {
                    if(firstAbove && it->first.second.first < it->first.first.first) {
                        if(parity%2 != 0) {
                            // push below segment to above
                            adjList[it->first.first].push_back(make_pair(prevAbove.first.second.first, it->first.first.second));
                            adjList[make_pair(prevAbove.first.second.first, it->first.first.second)].push_back(it->first.first);
                            segAdj[i][make_pair(it->first.first, make_pair(prevAbove.first.second.first, it->first.first.second))] = prevAbove.second;
                            segAdj[prevAbove.second][make_pair(make_pair(prevAbove.first.second.first, it->first.first.second), it->first.first)] = i;
                        }
                        parity++;
                    } else if(it->first.second.first >= it->first.first.first) {
                        firstAbove = true;
                        prevAbove = *it;
                        parity = 1;
                    }
                }
            }
        }

        void printAdjList() {
            for(map<pair<int, int>, vector<pair<int, int> > >::const_iterator it = adjList.begin();
                it != adjList.end(); ++it)
            {
                for(int i = 0; i < it->second.size(); i++) {
                    cout << it->first.first << ", " << it->first.second << " " << 
                    it->second[i].first << ", " << it->second[i].second << endl;
                }
            }
        }
};