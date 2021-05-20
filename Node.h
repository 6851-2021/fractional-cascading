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
        AugmentedCatalog<T> acatalog = AugmentedCatalog<T>();
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
            while(record != nullptr) {
                acatlogLookupTable[count]  = make_pair(record->getKey(),record);
                record = record->getUpPointer();
                count++;
            }
        }

        //Returns AugmentedRecord mapping to value if found, otherwise, returns successor
        pair<AugmentedRecord*,int> search(float value, int start, int end) {\
            if (end-start == 1) {
                float valueAtStart = acatlogLookupTable[start].first;
                float valueAtEnd = acatlogLookupTable[end].first;
                if (value==valueAtStart) {
                    return make_pair(acatlogLookupTable[start].second,start);
                } else if (value <= valueAtEnd){
                    return make_pair(acatlogLookupTable[end].second,end);
                }
                 else {
                     return make_pair(nullptr,-1); 
                } 
            }
            int mid = start+end/2;
            float valueAtMid = acatlogLookupTable[mid].first;
            if (valueAtMid == value) {
                return make_pair(acatlogLookupTable[mid].second,mid);
            } else if (valueAtMid < value) {
                return search(value,mid,end);
            } else {
                return search(value,start,mid);
            }
        }
        

};