#include "ConvexHull.h"
  
// Driver code
int main()
{
    iPair a[] = {make_pair(1, 1), make_pair(2, 3), make_pair(3, 1), make_pair(4, 3),
               make_pair(5, 1)};
    int n = sizeof(a)/sizeof(a[0]);
    cout<< "N:";
    cout << n;
    set<iPair> res = printHull(a, n);
    for (auto point:res){
        cout << point.first << "," << point.second << "\n";
    }
    return 0;
}
