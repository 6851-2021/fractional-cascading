#include "Record.h"

using namespace std;

template <typename T>

/**
 * @brief A record stored in an augmented catalog
 * 
 */
class AugmentedRecord : public Record {
    /**
     * AF(a_succesor, a_predecessor, flag) = A record stored in an augmented catalog
     *      where a_successor and a_predecessor are the next and previous elements
     *      in A_v, respectively, for this record. The flag is used to represent different
     *      states of the record during construction and maintaing of the record.
     */
    private:
        AugmentedRecord<T>* a_successor;
        AugmentedRecord<T>* a_predecessor;
        bool flag;
    public:
        AugmentedRecord(int key, Record<T>* c_succesor, AugmentedRecord<T>* a_successor, AugmentedRecord<T>* a_predecessor, bool flag) :
            Record(key, c_successor) {
                this->a_successor = a_successor;
                this->a_predecessor = a_predecessor;
                this->flag = flag;
            }
        
        /**
         * @brief Get the pointer to the successor in the augmented record
         * @return Record<T>* the successor or NIL if last
         */
        AugmentedRecord<T>* getASuccessor() {
            return this->a_successor;
        }

        /**
         * @brief Get the pointer to the predecessor in the augmented record
         * @return Record<T>* the predecessor or NIL if first
         */
        AugmentedRecord<T>* getAPredecessor() {
            return this->a_predecessor;
        }

        /**
         * @brief Get the Flag object
         * @return true if... TODO: not sure what flag is for yet
         * @return false if...  TODO: same as above
         */
        bool getFlag() {
            return this->flag;
        }

        /**
         * @brief Set the Flag object
         * @param new_flag what to set the flag to
         */
        void setFlag(bool new_flag) {
            this->flag = new_flag;
        }
};