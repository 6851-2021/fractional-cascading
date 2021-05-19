#pragma once

#include "Record.h"
#include <stddef.h>

using namespace std;


/**
 * @brief A record stored in an augmented catalog(A_v)
 * 
 */
class AugmentedRecord {
    /**
     * AF(key,c_pointer, up_pointer,down_pointer,flag) = A record stored in an augmented catalog
     *      where a_successor and a_predecessor are the next and previous elements
     *      in A_v, respectively, for this record. The flag is used to represent different
     *      states of the record during construction and maintaing of the record.
     */
    private:
        float key;
        Record* c_pointer;
        AugmentedRecord* up_pointer = NULL;
        AugmentedRecord* down_pointer = NULL;
        bool flag;
        bool isBridge = false;
    public:
        AugmentedRecord(float key, Record* c_pointer, bool flag) {
                this->key = key;
                this->c_pointer = c_pointer;
                this->flag = flag;
            }

        AugmentedRecord(float key, Record* c_pointer, bool flag, bool bridge) {
                this->key = key;
                this->c_pointer = c_pointer;
                this->flag = flag;
                this->isBridge = bridge;
            }
        
        AugmentedRecord(float key, Record* c_pointer, AugmentedRecord* up_pointer, AugmentedRecord* down_pointer, 
                bool flag) {
            this->key = key;
            this->c_pointer = c_pointer;
            this->up_pointer = up_pointer;
            this->down_pointer = down_pointer;
            this->flag = flag;
            }

        virtual ~AugmentedRecord() {}

        float getKey() {
            return this->key;
        }

        Record* getCPointer() {
            return this->c_pointer;
        }

        /**
         * @brief Get the pointer to the successor in the augmented record
         * @return Record<T>* the successor or NIL if last
         */
        AugmentedRecord* getUpPointer() {
            return this->up_pointer;
        }

        /**
         * @brief Get the pointer to the predecessor in the augmented record
         * @return Record<T>* the predecessor or NIL if first
         */
        AugmentedRecord* getDownPointer() {
            return this->down_pointer;
        }

                /**
         * @brief Get the pointer to the successor in the augmented record
         * @return Record<T>* the successor or NIL if last
         */
        void setUpPointer(AugmentedRecord* upPointer) {
            this->up_pointer = upPointer;
        }

        /**
         * @brief Get the pointer to the predecessor in the augmented record
         * @return Record<T>* the predecessor or NIL if first
         */
        void setDownPointer(AugmentedRecord* downPointer) {
           this->down_pointer = downPointer;
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

        bool getBridge() {
            return this->isBridge;
        }

};