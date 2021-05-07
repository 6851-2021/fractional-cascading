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
        int key;
        Record* up_pointer = NULL;
        bool endOfRange;
    public:
        Record(int key, bool endOfRange) {
            this->key = key;
            this->endOfRange = endOfRange;
        }


        /**
         * @brief Get the Key object
         * @return int the key (if the key is infinity, it is the last record
         *          of its catalog)
         */
        int getKey() {
            return this->key;
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