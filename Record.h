using namespace std;

template <typename T>

/**
 * @brief A record, as specified in the paper, stored in
 *        a normal (non-augmented) catalog.
 */
class Record {
    /**
     * AF(key, c_successor) = the record from a catalog with key 'key'
     *      and 'c_successor' is a pointer to the successor of the record
     *      in its own catalog
     * 
     */
    private:
        int key;
        Record<T>* c_successor;
    public:
        Record(int key, Record<T>* c_successor) {
            this->key = key;
            this->c_successor = c_successor;
        }

        /**
         * @brief Get the Key object
         * @return int the key (if the key is infinity, it is the last record
         *          of its catalog)
         */
        int getKey() {
            return this->key;
        }

        /**
         * @brief Get the pointer to the successor
         * @return Record<T>* the record (if NIL, it is the last record of its catalog)
         */
        Record<T>* getCSuccessor() {
            return this->c_successor;
        }
};