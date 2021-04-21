#include "Record.h"

#include <set>
#include <list>

using namespace std;

template <typename T>

/**
 * @brief Catalogs as described in paper (refered to as C_v).
 *        Stores the records of the data structure
 */
class Catalog {
    /**
     * AF(listOfRecords) = the catalog which holds the records in the order
     *                      specified in listOfRecords
     */
    private:
        set<Record<T>> setOfRecords;
        list<Record<T>> listOfRecords;
    public:
        Catalog(set<Record<T>> records) {
            this->setOfRecords = records;
        };

        Catalog(list<Record<T>> records) {
            this->listOfRecords = records;
        }

        /**
         * @brief Searches for a record in the catalog by key
         * @param key 
         * @return Record<T> the record if found, and nullptr if not
         */
        Record<T> search(int key) {
            for (Record<T> record : this->listOfRecords) {
                // CHANGE THIS TO SETOFRECORDS IF WE USE THAT REP
                if (record.getKey() == key) {
                    return record;
                }
            }
            return nullptr;
        };
};