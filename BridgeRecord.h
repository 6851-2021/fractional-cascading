#include "AugmentedRecord.h"
#include "Edge.h"

using namespace std;

template <typename T>

/**
 * @brief Bridge records -- TODO: TBH don't remember what they are
 * 
 */
class BridgeRecord : public AugmentedRecord {
    /**
     * AF()
     *      TODO: idk what this is lol
     */
    private:
        BridgeRecord<T>* prev_bridge;
        BridgeRecord<T>* companion_bridge;
        Edge edge;
        int count;
        int rank;
    public:
        BridgeRecord(int key, Record<T>* c_succesor, AugmentedRecord<T>* a_successor, AugmentedRecord<T>* a_predecessor, bool flag,
            BridgeRecord<T>* prev_bridge, BridgeRecord<T>* companion_bridge, Edge edge, int count, int rank) :
                AugmentedRecord(key, c_successor, a_successor, a_predecessor, flag) {
                    this->prev_bridge = prev_bridge;
                    this->companion_bridge = companion_bridge;
                    this->edge = edge;
                    this->count = count;
                    this->rank = rank;
                }

        /**
         * @brief Get the Prev Bridge object
         * @return BridgeRecord<T>* the lesser value bridge or NIL if lowest endpoint of range
         */
        BridgeRecord<T>* getPrevBridge() {
            return this->prev_bridge;
        }

        /**
         * @brief Get the Companion Bridge object
         * @return BridgeRecord<T>* the companion bridge
         */
        BridgeRecord<T>* getCompanionBridge() {
            return this->companion_bridge;
        }

        /**
         * @brief Get the Edge object
         * @return Edge 
         */
        Edge getEdge() {
            return this->edge;
        }

        /**
         * @brief The number of records in the augmented catalog that belong
         *        to the gap of which this record is the upper bridge. 0 if 
         *        this is the lowest bridge. The sum of this and the count
         *        field of the companion bridge is the gap size of this bridge.
         * @return int the count
         */
        int getCount() {
            return this->count;
        }

        /**
         * @brief Get the Gap size
         * @return int 
         */
        int getGapSize() {
            return this->companion_bridge->getCount() + this->getCount();
        }

        /**
         * @brief Get the Rank object
         * @return int the current rank
         */
        int getRank() {
            return this->rank;
        }

        /**
         * @brief Set the Rank object to new value
         * @param new_rank value to set the rank object to
         */
        void setRank(int new_rank) {
            this->rank = new_rank;
        }
};