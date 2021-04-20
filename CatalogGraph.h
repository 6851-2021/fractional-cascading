#include "Edge.h"
#include "Node.h"

#include <set>
#include <map>

using namespace std;

template <typename T>

class CatalogGraph {
    private:
        set<Node> nodes_;
        set<Edge> edges_;
    public:
        CatalogGraph(map<T, Record*> nodes, map<T, int[][]> edges) {

        };
        set<Node> getNodes() {
            return nodes_;
        }

        set<Edge> getEdges() {
            return edges_;
        }

};