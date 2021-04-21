#include "AugmentedRecord.h"
#include "Catalog.h"

#include <list>

using namespace std;

template <typename T>

/**
 * NOT SURE WHAT TO MAKE OF THIS CLASS IF EVEN NECESSARY
 * 
 * paper says on page 140, "The structure for A_v is more complex.
 *  It can be described as a doubly-linked list of records containing
 *  cross-reference to the records in C_v and with additional information
 *  stored in nodes that are bridges"
 */
class AugmentedCatalog : public Catalog {

};