#include "Catalog.h"
#include "AugmentedCatalog.h"

#include <set>

using namespace std;

template <typename T>

class Node {
    private:
        Catalog catalog;
        T label;
        AugmentedCatalog acatalog  = new AugmentedCatalog();
    public:
        Node(T label, Catalog catalog) {
            this->catalog = catalog;
            this->label = label;
        }

};