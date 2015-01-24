#include "edgeStream.h"
#include <cstring>

void testEarliestArrivalTime(esGraph g);
void testSSSTReachable(esGraph g);

int main()
{
	esGraph g;
	string fileName = "../data/data";
	g.buildGraph(fileName.c_str());
    
//	testEarliestArrivalTime(g);
	testSSSTReachable(g);

	return 0;
}

void testEarliestArrivalTime(esGraph g)
{
	vector<int> vTime;
	g.earliestArrivalTime(30, 20, 1000, vTime);
}

void testSSSTReachable(esGraph g)
{
	vector<int> vTime;
	g.SSSTReachable(30, 500, 20, 1000, vTime);
}
