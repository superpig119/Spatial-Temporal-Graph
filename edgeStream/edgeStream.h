#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

#define INFINITE 10000000

using namespace std;

typedef struct NODEINFO
{
	int x;
	int y;
	int t1;
	int t2;
}nodeInfo;

typedef struct EDGEINFO
{
	int u;
	int v;
	int t1;
	int t2;
}edgeInfo;

typedef struct NODE
{
	int id;
	vector<nodeInfo> vNodeInfo;
}node;

class esGraph
{
public:
	void buildGraph(const char* filename);
	void earliestArrivalTime(int u, int ts, int te, vector<int> &vTime);
	int nodePos(int id, int t, int &x, int &y);
	int nodeDist(int id1, int id2, int t, int &dist);
//	bool spatialDirectReachable(int id1, int id2, int esp, int t1, int t2, vector<pair<int, int> > &vpTime);
	void SSSTReachable(int u, int esp, int ts, int te, vector<int> &vTime);	//single source spatio-temporal reachability
	bool timeRangeDistReachable(int id1, int id2, int t1, int t2, int esp);
	void intersect(vector<pair<int, int> > vp, int t1, int t2, vector<pair<int, int> > &rvp);
	bool spatialDirectReachable(int id1, int id2, int esp, int t1, int t2, vector<pair<int, int> > &vpTime);

private:
	vector<node> vNode;
	vector<edgeInfo> vEdgeStream;
};

