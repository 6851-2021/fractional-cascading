#include "Record.h"

#include <set>
#include <list>
#include <map>

using namespace std;

float inf = numeric_limits<int>::max();
float neg_inf = -1*inf;


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
        Catalog() {

        }

        Record* getBottomRecord() {
            return this->bottomRecord;
        }

        void setBottomRecord(Record* bottomRecord) {
            this->bottomRecord = bottomRecord;
        }

};