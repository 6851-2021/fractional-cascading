#include "Record.h"

#include <set>
#include <list>
#include <map>

using namespace std;

int inf = numeric_limits<int>::infinity();
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
        list<Record> listOfRecords;
    public:
        /** Initializes a catalog with records from record sorted by key.
         */
        Catalog(list<Record> records) {
            records.sort();
            Record default_neginfinity(neg_inf);
            Record curr_record = records.front();
            records.pop_front();
            Record prev_record = default_neginfinity;
            listOfRecords.push_back(prev_record);
            while (records.size() != 0) {
                prev_record.setUpPointer(&curr_record);
                listOfRecords.push_back(curr_record);
                prev_record = curr_record;
                curr_record = records.front();
                records.pop_front();
            }
            Record default_infinity(inf);
            curr_record.setUpPointer(&default_infinity);
        }

        /** Initializes an "empty" catalog with the default records of negative infinity and infinity.
         */
        Catalog() {
            Record default_neginfinity(neg_inf);
            Record default_infinity(inf);
            default_neginfinity.setUpPointer(&default_infinity);
            listOfRecords.push_back(default_neginfinity);
            listOfRecords.push_back(default_infinity);
        }

        /**
         * @brief Searches for a record in the catalog by key
         * @param key 
         * @return Record<T> the record if found, and nullptr if not
         */
        Record search(int key) {
            for (Record record : this->listOfRecords) {
                // CHANGE THIS TO SETOFRECORDS IF WE USE THAT REP
                if (record.getKey() == key) {
                    return record;
                }
            }
            return NULL;
        };
};