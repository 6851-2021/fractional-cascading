#include <iostream>
#include <fstream>
#include <utility>
#include <list>
#include <deque>
#include <cmath>
#include <map>
#include <queue>
#include <vector>
#include "ConvexHull.h"
#include "AugmentedRecord.h"
#include "CatalogGraph.h"
 
#define PI 3.14159265
#define Point pair<float,float>
#define Line pair<Point,Point>

using namespace std;

//Like memo from DP
//This also stores the convex hull in reverse order since the points are sorted  x
map<int,pair<Line*,int> > nodeDictionary; //stores node label and the lines/edges of the chain associated with it
map<int,list<int> >  nodesToIncidentEdges;
map<int,pair<int,int> > edgeDictionary; //stores the edge label and the pair of node labels associated with the edge
map<int,pair<vector<Line>, int> > convexHulls; //stores node label to the convex hull of the sub-chain it represents
//Eventually be fed into the Catalog Graph
map<int,list<float> > slopeSequence;
map<int,pair<float,float> > edgeRanges;

//use arrays instead of pointers!!!
void getNodesAndEdges(Line* polygonPath, int sizeOfArray){
    //Initialise  queue for constructing nodes
    deque< pair <pair<Line*,int>,int> > buildQueue; //holds pairs of nodes(as described by start of array and size) and their integer labels
    buildQueue.push_back(make_pair(make_pair(polygonPath,sizeOfArray),0));
    int edge_count  = 0; //to keep track of the keys on edges
    while(!buildQueue.empty()){
        //Step 1: remove a node for processing from the queue
        pair<pair<Line*,int>,int> node_and_label = buildQueue.front();
        buildQueue.pop_front();


        //Step 2: extract the relevant parts
        int node_label = node_and_label.second;
        int arraySize = node_and_label.first.second;
        Line* arrayPointer = node_and_label.first.first;
        nodeDictionary[node_label] = node_and_label.first;

        //Add its children to the queue if it has > 2 points.
        if (arraySize <= 1){
            continue;
        } else  {
            int mid = arraySize/2; //number of lines
            pair<Line*,int> left_child = make_pair(arrayPointer,mid);
            pair<Line*,int> right_child = make_pair(arrayPointer+mid,arraySize-mid);
            int left_child_label = (2*node_label) + 1;
            int right_child_label = (2*node_label)+2;
            buildQueue.push_back(make_pair(left_child,left_child_label));
            buildQueue.push_back(make_pair(right_child,right_child_label));
            //Include this in the edge dictionary
            edgeDictionary[edge_count] = make_pair(node_label,left_child_label);
            edgeDictionary[edge_count+1] = make_pair(node_label,right_child_label);
            nodesToIncidentEdges[node_label].push_back(edge_count);
            nodesToIncidentEdges[node_label].push_back(edge_count+1);
            edge_count+=2;
        }
    }

}

pair<pair<float,float>*,pair<float,float>*> getHighestLowestPoint(pair<float,float>* polygonalPath,int sizeOfArray){
    pair<float,float>* bestSoFarHigh = polygonalPath;
    int bestHigh = (*polygonalPath).second;
    pair<float,float>* bestSoFarLow = polygonalPath;
    int bestLow = (*polygonalPath).second;
    for (int i= 0;i<sizeOfArray;i++){
        if ((*(polygonalPath+i)).second > bestHigh) {
            bestHigh = (*(polygonalPath+i)).second;
            bestSoFarHigh = polygonalPath+i;
        }
        if ((*(polygonalPath+i)).second < bestLow){
            bestLow = (*(polygonalPath+i)).second;
            bestSoFarLow =  polygonalPath+i;
        }
    }
    return make_pair(bestSoFarLow,bestSoFarHigh);
}

float averageY(pair<float,float>* polygonalPath,int sizeOfArray){
    float res = 0;
    for (int i=0; i<sizeOfArray;i++){
        res+= (polygonalPath+i)->second;
    }
    return res/sizeOfArray;
}
float averageX(pair<float,float>* polygonalPath,int sizeOfArray){
    float res = 0;
    for (int i=0; i<sizeOfArray;i++){
        res+= (polygonalPath+i)->first;
    }
    return res/sizeOfArray;
    
}

map<char,float> computeABC(pair<float,float> p1, pair<float,float> p2, pair<float,float> p3)  {
    //a  is p1p2, b is p2p3, c is p3p1
    float a = pow(pow((p1.first-p2.first),2) + pow((p1.second-p2.second),2),0.5);
    float b = pow(pow((p2.first-p3.first),2) + pow((p2.second-p3.second),2),0.5);
    float c = pow(pow((p3.first-p1.first),2) + pow((p3.second-p1.second),2),0.5);
    map<char,float> result;
    result['a'] = a;
    result['b'] = b;
    result['c'] = c;
    return  result;
}

bool custom_compare(const pair<int, float>& p1, const pair<int, float>& p2){
    return p1.second <= p2.second; 
}

//Reorders points such that they are ordered in clockwise fashion
vector<Point> reorderConvexHullClockwise(Point* polygonalPath,int sizeOfArray){
    //Step 1: Choose a point in the polygon
    float avg_y = averageY(polygonalPath,sizeOfArray);
    float avg_x = averageX(polygonalPath,sizeOfArray);
    pair<float,float> midPoint = make_pair(avg_x,avg_y);

    //Chose a second point for line
    pair<float,float>* highestPoint = getHighestLowestPoint(polygonalPath,sizeOfArray).second;
    float highestY = highestPoint->second;
    pair<float,float> secondPoint = make_pair(avg_x+0.001,highestY+10);

    // float m_1 = (midPoint.second-secondPoint.second)/(midPoint.first-secondPoint.first);
    pair<int,float> angles[sizeOfArray];//index of point and angle
    for (int i=0;i<sizeOfArray;i++){
        pair<float,float> currPoint = *(polygonalPath+i);
        map<char,float> triangleLengths = computeABC(secondPoint,midPoint,currPoint);
        float a = triangleLengths['a'];
        float b = triangleLengths['b'];
        float c = triangleLengths['c'];
        float angle = (180*acos((pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b)))/PI;
        //Take the acute angle if line is to the right
        if (currPoint.first>=midPoint.first){
            angles[i] = make_pair(i,angle);
        }
        //subtract from 360 to get angle if line is to the left
        else {
            angles[i] = make_pair(i,360-angle);
        }
    }
    sort(angles,angles+sizeOfArray,custom_compare);
    vector<Point> result(sizeOfArray);
    for (int i=0; i<sizeOfArray; i++){
        int index =  angles[i].first;
        result[i] = *(polygonalPath+index);

    }
    return result;
}

void computeSlopeSequences(){
    for (auto const& node_and_ch : convexHulls){
        int nodeKey = node_and_ch.first;
        vector<Line> convexHull = node_and_ch.second.first;
        int nLinesInCH = node_and_ch.second.second;
        list<float> slopes;
        if (nLinesInCH == 1) {
            slopes.push_back(-1);
        } else {
            for (int i=0;i<nLinesInCH;i++){
                Line side_i = convexHull[i];
                Point c_i = side_i.first;
                Point c_i_1 = side_i.second;
                Point pos_inf_ray = make_pair(1000,c_i.second);
                map<char,float> sidesOfTriangle = computeABC(c_i_1,c_i,pos_inf_ray);
                float  a = sidesOfTriangle['a'];
                float  b = sidesOfTriangle['b'];
                float  c = sidesOfTriangle['c'];
                float angle_C = (180*acos((pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b)))/PI;
                slopes.push_back(angle_C);
            }
        }
        slopes.sort();
        slopeSequence[nodeKey] = slopes;
    }
}

void getSlopeEdgeRanges(){
    for (auto const& edge_and_endpoints: edgeDictionary){
        int edgeKey = edge_and_endpoints.first;
        pair<int,int> endPoints = edge_and_endpoints.second;
        int endPoint1 = endPoints.first;
        int endPoint2 = endPoints.second;
        list<float> slopeSequence1 = slopeSequence[endPoint1];
        list<float> slopeSequence2 = slopeSequence[endPoint2];
        if (slopeSequence1.size()>slopeSequence2.size()){
            edgeRanges[edgeKey] = make_pair(slopeSequence2.front(),slopeSequence2.back());
        } else {
            edgeRanges[edgeKey] = make_pair(slopeSequence1.front(),slopeSequence1.back());
        }
    }
}

bool checkIntersect(Line line1, Line line2) {
        float m = (line1.first.second-line1.second.second)/(line1.first.first-line1.second.first);
        float c = line1.first.second - (m*line1.first.first);
        int signfirst;
        int signsecond;
        if (line2.first.second > (line2.first.first*m+c)) {
            signfirst = 1;
        } else {
            signfirst = -1;
        }
        if (line2.second.second > (line2.second.first*m+c)) {
            signsecond = 1;
        } else {
            signsecond = -1;
        }
        return signsecond != signfirst;
}

int  main() {
    //Initialise poligonal chain and query line
    int POLYGON_PATH_NUMBER_OF_LINES=  5;
    Line polygonPath[POLYGON_PATH_NUMBER_OF_LINES]; 
    polygonPath[0] = make_pair(make_pair(1,1),make_pair(2,3));
    polygonPath[1] = make_pair(make_pair(2,3),make_pair(3,1));
    polygonPath[2] = make_pair(make_pair(3,1),make_pair(4,3));
    polygonPath[3] =  make_pair(make_pair(4,3),make_pair(5,1));
    polygonPath[4] = make_pair(make_pair(5,1), make_pair(6,3));
    Line l = make_pair(make_pair(1.2,2.5),make_pair(2.5,2.8));

    //Compute s and s' of l
    Point p1_2 = make_pair(5,l.second.second);
    map<char,float> sides_2 = computeABC(p1_2,l.first,l.second);
    float a_2 = sides_2['a'];
    float b_2 = sides_2['b'];
    float c_2 = sides_2['c'];
    int s = (180*acos((pow(a_2,2)+pow(b_2,2)-pow(c_2,2))/(2*a_2*b_2)))/PI;
    Point p1 = make_pair(5,l.first.second);
    map<char,float> sides = computeABC(p1,l.first,l.second);
    float a = sides['a'];
    float b = sides['b'];
    float c = sides['c'];
    float s_prime = (180*acos((pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b)))/PI;

    //Open file for writing results to be used for visualization
    ofstream resultsFile;
    resultsFile.open("visual.txt");
    resultsFile << "(" << l.first.first << "," << l.first.second << "," << l.second.first << "," << l.second.second << ")" << "\n";
    for (int i=0; i< POLYGON_PATH_NUMBER_OF_LINES;i++){
        resultsFile << "(" << polygonPath[i].first.first << "," << polygonPath[i].first.second << "," << polygonPath[i].second.first << "," << polygonPath[i].second.second << ")" << " ";
    }
    resultsFile << "\n";
    
    //Step 1: Organize into nodes and edges
    getNodesAndEdges(&(polygonPath[0]),POLYGON_PATH_NUMBER_OF_LINES);
    for (auto const& node_and_array : nodeDictionary) {
        int nodeKey = node_and_array.first;
        pair<Line*,int> nodeArray = node_and_array.second;
        int nodeArraySize = nodeArray.second;
        Line* curPoint = nodeArray.first;
        if (nodeArraySize <=1){
            vector<Line> line(1);
            line[0] = *(curPoint);
            convexHulls[nodeKey] = make_pair(line,nodeArraySize);
        }  else {
                //Create a point array for computing convex hull
                Point pointArray[nodeArraySize+1];
                int i = 0;
                while (i<nodeArraySize){
                    Line line = *(curPoint+i);
                    pointArray[i] = line.first;
                    i++;
                }
                pointArray[i] = (*(curPoint+nodeArraySize-1)).second; //last point on line
                //Compute  convex hull and place results in an array
                set<Point> res = printHull(pointArray,nodeArraySize+1);
                Point convArray[res.size()];
                int j = 0;
                for (Point point:res){
                    convArray[j] = point;
                    j++;
                }
                vector<Point> reordered = reorderConvexHullClockwise(convArray,j);
                vector<Line> finalResult(j);
                for (int k=0;k<j;k++){
                    int index1 = k%j;
                    int index2 = (k+1)%j;
                    finalResult[k] = make_pair(reordered[index1],reordered[index2]);
                    //cout <<  "(" << finalResult[k].first.first << ","  << finalResult[k].first.second << ")"<<  "& (" << finalResult[k].second.first << ","  << finalResult[k].second.second << ")"<< "\n";
                }
                convexHulls[nodeKey] = make_pair(finalResult,j);
                }
    }
    //Compute relevant information
    computeSlopeSequences();
    getSlopeEdgeRanges();

    //Construct Catalog graph and perform search
    CatalogGraph<int>* CGraph = new CatalogGraph<int>(slopeSequence,edgeDictionary,edgeRanges,2);
    CGraph->constructAugmentedCatalogs();
    map<int,Node<int>*> nodes = CGraph->getMapOfNodes();
    map<int,BridgeRecord<int>*> nodeToBridges;

    //Step 1: Search in root if line slope can be found
    Node<int>* root = nodes[0];
    int size = nodeDictionary[0].second;
    root->createLookupTable();
    pair<AugmentedRecord*,int> res1 = root->search(s,0,size-1);
    pair<AugmentedRecord*,int> res2 = root->search(s_prime,0,size-1);
    
    //Initialise result holders
    int res = 0;
    list<Line> edgesIntersected;

    // Slope of line is not in range so output no intersection
    if (res1.first == nullptr || res2.first == nullptr){
        cout << "No intersection";
    }
    //If in range, search tree
    else {
        queue< pair<int,pair<AugmentedRecord*,AugmentedRecord*> > > searchQueue; //queue containing node with augmentedRecordPointer and its key
        pair<int,pair<AugmentedRecord*,AugmentedRecord*> > first_entry = make_pair(0,make_pair(res1.first,res2.first)); // include both s and s' in search
        searchQueue.push(first_entry);
        int count  = 0;
        while (!searchQueue.empty()){
            //Step 1: Pop an element from the queue
            pair<int,pair<AugmentedRecord*,AugmentedRecord*> > currNode = searchQueue.front();
            searchQueue.pop();

            //Step 2: Extract relevant info
            int nodeKey = currNode.first;
            list<int> incidentEdges = nodesToIncidentEdges[nodeKey];
            int left_edge = incidentEdges.front();
            int right_edge = incidentEdges.back();
            AugmentedRecord* searchResult_s = currNode.second.first;
            AugmentedRecord* searchResult_sprime = currNode.second.second;
            int left_child = 2*nodeKey+1;
            int right_child = 2*nodeKey+2;
            Line* p = nodeDictionary[left_child].first;
            for (int i=0;i<nodeDictionary[left_child].second;i++) {
                resultsFile << "(" << (*(p+i)).first.first << "," << (*(p+i)).first.second << "," << (*(p+i)).second.first << "," << (*(p+i)).second.second << ")" << " ";
            }
            resultsFile << "\n";
            if (nodeDictionary[left_child].second == 1){
                if (checkIntersect(l,*nodeDictionary[left_child].first)){
                    res+=1;
                }
            } 
            else  {
                AugmentedRecord* left_searchResult_s = CGraph->findAugRecord(searchResult_s,left_edge);
                AugmentedRecord* left_searchResult_sprime = CGraph->findAugRecord(searchResult_sprime,left_edge);
                if (left_searchResult_s && left_searchResult_s){
                    pair<int,pair<AugmentedRecord*,AugmentedRecord*> > left_entry = make_pair(left_child,make_pair(left_searchResult_s,left_searchResult_sprime));
                    searchQueue.push(left_entry);
                }
            }
            Line* q = nodeDictionary[right_child].first;
            for (int i=0;i<nodeDictionary[right_child].second;i++) {
                resultsFile << "(" << (*(q+i)).first.first << "," << (*(q+i)).first.second << "," << (*(q+i)).second.first << "," << (*(q+i)).second.second << ")" << " ";
            }
            resultsFile << "\n";
            if (nodeDictionary[right_child].second==1){
                if (checkIntersect(l,*nodeDictionary[right_child].first)){
                    res+=1;
                }
            }
            else {
                AugmentedRecord* right_searchResult_s = CGraph->findAugRecord(searchResult_s,right_edge);
                AugmentedRecord* right_searchResult_sprime = CGraph->findAugRecord(searchResult_sprime,right_edge);
                if (right_searchResult_s && right_searchResult_sprime){
                    pair<int,pair<AugmentedRecord*, AugmentedRecord*> > right_entry = make_pair(right_child,make_pair(right_searchResult_s,right_searchResult_sprime));
                    searchQueue.push(right_entry);
                }
            }
        }
    }
    resultsFile.close();
    cout << "Number of Intersections:";
    cout  << res;
}

    