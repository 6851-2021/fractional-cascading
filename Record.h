#pragma once
#include <stddef.h>
using namespace std;


/**
 * @brief A record, as specified in the paper, stored in
 *        a normal (non-augmented) catalog.
 */
class Record {
    /**
     * AF(key, up_pointer) = the record from a catalog with key 'key'
     *      and 'c_successor' is a pointer to the successor of the record
     *      in its own catalog
     * 
     */
    private:
        float key;
        Record* up_pointer = NULL;
        bool endOfRange;
        int edge = -1;
    public:
        Record(float key, bool endOfRange) {
            this->key = key;
            this->endOfRange = endOfRange;
        }

        Record(float key, bool endOfRange,float edge_label) {
            this->key = key;
            this->endOfRange = endOfRange;
            this->edge = edge_label;
        }


        /**
         * @brief Get the Key object
         * @return float the key (if the key is infinity, it is the last record
         *          of its catalog)
         */
        float getKey() {
            return this->key;
        }

        int getEdge() {
            return this->edge;
        }

        bool getEndOfRange() {
            return this->endOfRange;
        }

        Record* getUpPointer() {
            return this->up_pointer;
        }

        void setUpPointer(Record* up_pointer) {
            this->up_pointer = up_pointer;
        }
};