CXX = g++ -std=c++11

all:
	$(CXX) -o CatalogGraphTest CatalogGraphTest.cpp
	$(CXX) -o ConvexHull ConvexHull.cpp
	$(CXX) -o HiveGraphTest HiveGraphTest.cpp
	$(CXX) -o PolygonalPathIntersection PolygonalPathIntersection.cpp
