#include "Record.h"

#include <set>
#include <list>
#include <map>

using namespace std;

int inf = numeric_limits<int>::max();
int neg_inf = -1*inf;


/**
 * @brief Catalogs as described in paper (refered to as C_v).
 *        Stores the records of the data structure
 */
class Catalog {
    /**
     * AF(listOfRecords) = the catalog which holds the records in listOfRecords
     *                      in sorted order
     */
    private:
        Record* bottomRecord = NULL;
    public:

        /** Initializes an "empty" catalog with the default records of negative infinity and infinity.
         */
        Catalog() {

        }

        Record* getBottomRecord() {
            return this->bottomRecord;
        }

        void setBottomRecord(Record* bottomRecord) {
            this->bottomRecord = bottomRecord;
        }
        // /**
        //  * @brief Searches for a record in the catalog by key
        //  * @param key 
        //  * @return Record<T> the record if found, and nullptr if not
        //  */
        // Record* search(int key) {
        //     for (Record record : this->listOfRecords) {
        //         // CHANGE THIS TO SETOFRECORDS IF WE USE THAT REP
        //         if (record.getKey() == key) {
        //             return &record;
        //         }
        //     }
        //     return nullptr;
        // };
};