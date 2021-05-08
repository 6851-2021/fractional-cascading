#include "Catalog.h"
#include "AugmentedCatalog.h"

#include <map>
#include <utility>

using namespace std;

template <typename T>

class Node {
    private:
        Catalog catalog = new Catalog();
        T label;
        AugmentedCatalog acatalog  = new AugmentedCatalog();
        map<int, pair<int,AugmentedRecord*> > acatlogLookupTable;
    public:
        Node(T label) {
            this->label = label;
        }

        Catalog* getCatalog() {
            return &this->catalog;
        }

        AugmentedCatalog*  getAugCatalog() {
            return &this->acatalog;
        }

        T getLabel() {
            return this->label;
        }


        void createLookupTable(int value) {
            int count = 0;
            for (AugmentedRecord record:acatalog) {
                acatlogLookupTable[count]  = make_pair(record.getKey(),&record);
                count++;
            }
        }

        AugmentedRecord* search(int value, int start, int end) {
            if (start-end == 1) {
                int valueAtStart = acatlogLookupTable[start].first;
                int valueAtEnd = acatlogLookupTable[end].first;
                if (value==valueAtStart) {
                    return acatlogLookupTable[start].second;
                } else if (value == valueAtEnd) {
                    return acatlogLookupTable[end].second;
                } else {
                    return nullptr;
                }
            }
            int mid = start+end/2;
            int valueAtMid = acatlogLookupTable[mid].first;
            if (valueAtMid == value) {
                return acatlogLookupTable[mid].second;
            } else if (valueAtMid < value) {
                return search(value,mid,end);
            } else {
                return search(value,start,mid);
            }
        }
        

};