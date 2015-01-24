#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include <queue>

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
	int id;
	int t1;
	int t2;
}edgeInfo;

typedef struct NODE
{
	int id;
	vector<nodeInfo> vnodeInfo;
	list<edgeInfo> ledgeInfo;
}node;

typedef struct PATHINFO
{
	int id;
	int t1;
	int t2;
}pathInfo;

class Graph
{
public:
	void buildGraph(const char* fileName);
	int nodeNum();
	int edgeNum();
	
	int nodePos(int id, int t, int &x, int &y);	//return the position of a node at time t
	int nodeDist(int id1, int id2, int t, int &dist);
	int nodeDistRange(int id1, int id2, int t1, int t2, vector<int> &vDistRange);
	bool spatialDirectReachable(int id1, int id2, int esp, int t1, int t2, vector<pair<int, int> > &vpTime);
	bool edgeExist(int id1, int id2, int t);
	int edgeExistTime(int id1, int id2, vector<pair<int, int> > &vpTime);
	void intersect(vector<pair<int, int> > vp, int t1, int t2, vector<pair<int, int> > &rvp);
	bool temporalDirectReachable(int id1, int id2, int t1, int t2, vector<pair<int, int> > &vpTime);
	bool STDReachable(int id1, int id2, int esp, int t1, int t2, vector<pair<int, int> > &vpTime);	//return the time intervals when the two nodes are spatio-temporal direct reachable
	bool STReachable(int id1, int id2, int esp, int t1, int t2, vector<pathInfo> vPathInfo);
	void showNeighbors(int id);
	void SSSTReachable(int u, int esp, int t1, int t2, vector<int> &vTime);	//single source earliest arrival time


private:
	vector<node> vnode;
};


