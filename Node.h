#pragma once

#include "Catalog.h"
#include "AugmentedCatalog.h"

#include <map>
#include <utility>

using namespace std;

template <typename T>

class Node {
    private:
        Catalog catalog = Catalog();
        T label;
        AugmentedCatalog<T> acatalog = AugmentedCatalog<T>();;
        map<int, pair<float,AugmentedRecord*> > acatlogLookupTable;
    public:
        Node(T label) {
            this->label = label;
        }

        Catalog* getCatalog() {
            return &this->catalog;
        }

        AugmentedCatalog<T>* getAugCatalog() {
            return &this->acatalog;
        }

        T getLabel() {
            return this->label;
        }


        void createLookupTable() {
            int count = 0;
            AugmentedRecord* record = acatalog.getBottomRecord();
            while(record) {
                acatlogLookupTable[count]  = make_pair(record->getKey(),record);
                record = record->getUpPointer();
                count++;
            }
        }

        AugmentedRecord* search(float value, int start, int end) {
            if (start-end == 1) {
                float valueAtStart = acatlogLookupTable[start].first;
                float valueAtEnd = acatlogLookupTable[end].first;
                if (value==valueAtStart) {
                    return acatlogLookupTable[start].second;
                } else if (value == valueAtEnd) {
                    return acatlogLookupTable[end].second;
                } else {
                    return nullptr;
                }
            }
            float mid = start+end/2;
            float valueAtMid = acatlogLookupTable[mid].first;
            if (valueAtMid == value) {
                return acatlogLookupTable[mid].second;
            } else if (valueAtMid < value) {
                return search(value,mid,end);
            } else {
                return search(value,start,mid);
            }
        }
        

};