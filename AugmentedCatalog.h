#include "AugmentedRecord.h"

#include <list>

using namespace std;

template <typename T>

/**
 * @brief Augmented catalogs as described in paper (refered to as A_v).
 *        Stores the augmented records of the data structure
 */
class AugmentedCatalog {
    /**
     * AF(listOfRecords) = the catalog which holds the augmented records in the order
     *          specified in listOfRecords
     */
    private:
        set<AugmentedRecord<T>> setOfRecords;
        list<AugmentedRecord<T>> listOfRecords;
    public:
        Catalog(set<AugmentedRecord<T>> records) {
            this->setOfRecords = records;
        };

        Catalog(list<AugmentedRecord<T>> records) {
            this->listOfRecords = records;
        }

        /**
         * @brief Searches for a record in the catalog by key
         * @param key 
         * @return Record<T> the record if found, and nullptr if not
         */
        AugmentedRecord<T> search(int key) {
            for (AugmentedRecord<T> record : this->listOfRecords) {
                // CHANGE THIS TO SETOFRECORDS IF WE USE THAT REP
                if (record.getKey() == key) {
                    return record;
                }
            }
            return nullptr;
        };
};