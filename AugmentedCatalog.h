#include "AugmentedRecord.h"
#include <stddef.h>
#include <optional>
#include <list>
#include <set>
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
        list<AugmentedRecord> listOfRecords;
    public:
    //     //TODO: Change this to an optional object
    //     /**
    //      * @brief Searches for a record in the catalog by key
    //      * @param key 
    //      * @return Record<T> the record if found, and nullptr if not
    //      */
    //    AugmentedRecord search(int key) {
    //         for (AugmentedRecord record : this->listOfRecords) {
    //             // CHANGE THIS TO SETOFRECORDS IF WE USE THAT REP
    //             if (record.getKey() == key) {
    //                 return record;
    //             }
    //         }
    //         return;
    //     };

        
        /**
         * @brief Inserts a record into A_v
         * @param p Record
         * @param d max degree of catalog graph
         * @return AugmentedRecord* pointer to the AugmentedReccord created
         */
        AugmentedRecord* insert(Record p, list<AugmentedRecord>::iterator it, bool bridge) {
            while (it != listOfRecords.end()) {
                if (it->getKey() > p.getKey()) {
                    break;
                }
                it++;
            }
            if(bridge) {
                BridgeRecord<T> newRecord(p.getKey(), &p, 0);
            } else {
                AugmentedRecord newRecord(p.getKey(),&p,0);
            }
            newRecord.setUpPointer(&(*it));
            newRecord.setDownPointer(it->getDownPointer());
            AugmentedRecord* toReasign = it->getDownPointer();
            toReasign->setUpPointer(&newRecord);
            it->setDownPointer(&newRecord);
            return &newRecord;    
        }


        // AugmentedCatalog(list<AugmentedRecord> records) {
        //     records.sort();
        //     AugmentedRecord prev_record = records.front();
        //     records.pop_front();
        //     AugmentedRecord curr_record = records.front();
        //     while (records.size() != 0) {
        //         prev_record.setUpPointer(&curr_record);
        //         curr_record.setDownPointer(&prev_record);
        //         listOfRecords.push_back(prev_record);
        //         prev_record = curr_record;
        //         curr_record = records.front();
        //         records.pop_front();
        //     }
        // }
};