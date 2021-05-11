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
        BridgeRecord* prev_bridge = NULL;
        BridgeRecord* companion_bridge = NULL;
        Edge<T> edge;
        int count = 0;
        int rank = 0;
    public:
        BridgeRecord(int key, Record* c_succesor, AugmentedRecord* a_successor, AugmentedRecord* a_predecessor, bool flag
            BridgeRecord* prev_bridge, BridgeRecord* companion_bridge, Edge<T> edge, int count, int rank) :
                AugmentedRecord(key, c_successor, a_successor, a_predecessor, flag) {
                    this->prev_bridge = prev_bridge;
                    if(companion_bridge){
                        this->companion_bridge = companion_bridge;
                    } else {
                        companion_bridge =
                    }
                    this->edge = edge;
                    this->count = count;
                    this->rank = rank;
                    this->isBridge = true;
                }

        /**
         * @brief Get the Prev Bridge object
         * @return BridgeRecord<T>* the lesser value bridge or NIL if lowest endpoint of range
         */
        BridgeRecord* getPrevBridge() {
            return this->prev_bridge;
        }

        /**
         * @brief Get the Companion Bridge object
         * @return BridgeRecord<T>* the companion bridge
         */
        BridgeRecord* getCompanionBridge() {
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

        void setCount(int new_count) {
            this->count = new_count;
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