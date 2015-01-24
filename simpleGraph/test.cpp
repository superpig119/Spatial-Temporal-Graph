#include "graph.h"
#include <cstring>

void testNodePos(Graph g);
void testNodeDist(Graph g);
void testNodeDistRange(Graph g);
void testSpatialDirectReachable(Graph g);
void testEdgeExistTime(Graph g);
void testTemporalDirectReachable(Graph g);
void testSTDReachable(Graph g);
void testSTReachable(Graph g);
void testSSSTReachable(Graph g);

int main()
{
	Graph g;
	string fileName = "../data/data";
	g.buildGraph(fileName.c_str());
	cout << "nodeNum:" << g.nodeNum() << endl;
	cout << "edgeNum:" << g.edgeNum() << endl;
    
//	testNodePos(g);
//  g.showNeighbors(3);
//	testNodeDist(g);
//	testNodeDistRange(g);
//	testSpatialDirectReachable(g);
//	testEdgeExistTime(g);
//	testTemporalDirectReachable(g);
//	testSTDReachable(g);
//	testSTReachable(g);
	testSSSTReachable(g);
	return 0;
}

void testNodePos(Graph g)
{
	int x, y, t;
	t = 1200;
	if(g.nodePos(3, t, x, y))
		cout << "Point 3's pos at " << t << " is (" << x << "," << y << ")" << endl;
}

void testNodeDist(Graph g)
{
	int dist;
	if(g.nodeDist(4, 6, 2000, dist))
		cout << "The distance between 4 and 6  at 2000 is " << dist << endl;
}

void testNodeDistRange(Graph g)
{
	int t1 = 1000;
	int t2 = 9000;
	int i;
	vector<int> vDistRange;
	vector<int>::iterator ivDistRange;
	g.nodeDistRange(500, 100, t1, t2, vDistRange);
	for(ivDistRange = vDistRange.begin(), i = 0; ivDistRange != vDistRange.end(); ivDistRange++, i++)
	{
		cout << "time " << t1 + i << " distance: " << *ivDistRange << endl;
	}
}

void testSpatialDirectReachable(Graph g)
{
	int id1 = 3;
	int id2 = 400;
	int esp = 50;
	int t1 = 600;
	int t2 = 800;
	vector<pair<int, int> > vpTime;
	vector<pair<int, int> >::iterator ivpTime;
	if(g.spatialDirectReachable(id1, id2, esp, t1, t2, vpTime))
		for(ivpTime = vpTime.begin(); ivpTime != vpTime.end(); ivpTime++)
			cout << (*ivpTime).first << "\t" << (*ivpTime).second << endl;
	else
		cout << id1 << " and " << id2 << " are not spatially reachable!" << endl;
}

void testEdgeExistTime(Graph g)
{
	int id1 = 3;
	int id2 = 400;
	vector<pair<int, int> > vpTime;
	vector<pair<int, int> >::iterator ivpTime;
	if(g.edgeExistTime(id1, id2, vpTime))
	{
		for(ivpTime = vpTime.begin(); ivpTime != vpTime.end(); ivpTime++)
		{
			cout << "Edge between " << id1 << " and " << id2 << " exists between " << (*ivpTime).first << " and " << (*ivpTime).second << endl;
		}
	}
}

void testTemporalDirectReachable(Graph g)
{
	int id1 = 3;
	int id2 = 400;
	int t1 = 600;
	int t2 = 800;
	vector<pair<int, int> > vpTime;
	vector<pair<int, int> >::iterator ivpTime;
	if(g.temporalDirectReachable(id1, id2, t1, t2, vpTime))
	{
		for(ivpTime = vpTime.begin(); ivpTime != vpTime.end(); ivpTime++)
		{
			cout << "Edge between " << id1 << " and " << id2 << " exists between " << (*ivpTime).first << " and " << (*ivpTime).second << endl;
		}
	}
}

void testSTDReachable(Graph g)
{
	int id1 = 3;
	int id2 = 400;
	int esp = 50;
	int t1 = 600;
	int t2 = 800;
	vector<pair<int, int> > vpTime;
	vector<pair<int, int> >::iterator ivpTime;
	if(g.STDReachable(id1, id2, esp, t1, t2, vpTime))
	{
		for(ivpTime = vpTime.begin(); ivpTime != vpTime.end(); ivpTime++)
		{
			cout << "STD Reachable between " << id1 << " and " << id2 << " from " << (*ivpTime).first << " to " << (*ivpTime).second << endl;
		}
	}
	else
		cout << id1 << " and " << id2 << " are not STD Reachable!" << endl;
}

void testSTReachable(Graph g)
{
	int id1 = 3;
	int id2 = 47;
	int esp = 400;
	int t1 = 600;
	int t2 = 700;
	vector<pathInfo> vPathInfo;
	if(g.STReachable(id1, id2, esp, t1, t2, vPathInfo))
		cout << "Reachable!" << endl;
	else
		cout << "Unreachable!" << endl;
}

void testSSSTReachable(Graph g)
{
	vector<int> vTime;
	vector<int>::iterator ivTime;
	int i;
	g.SSSTReachable(30, 500, 20, 1000, vTime);
	for(ivTime = vTime.begin(), i = 0; ivTime != vTime.end(); ivTime++, i++)
	{
		if(*ivTime != INFINITE)	
			cout << i << "\t"  << *ivTime << endl;
	}	
}
