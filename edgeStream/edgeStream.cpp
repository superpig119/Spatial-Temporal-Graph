#include "edgeStream.h"

bool eCompare(edgeInfo e1, edgeInfo e2)
{
	if(e1.t1 < e2.t1)
		return true;
	else if(e1.t1 == e2.t1)
	{
		if(e1.t2 <= e2.t2)
			return true;
		else
			return false;
	}

	return false;
}

void esGraph::buildGraph(const char* filename)
{
	ifstream infile(filename);
	int lineNum;
	infile >> lineNum;
	int i,j;
	int posNum;
	for(i = 0; i < lineNum; i++)
	{
		node n;
		infile >> n.id;
		infile >> posNum;
		for(j = 0; j < posNum; j++)
		{
			nodeInfo ni;
			infile >> ni.x >> ni.y >> ni.t1 >> ni.t2;
			n.vNodeInfo.push_back(ni);
		}
		vNode.push_back(n);
	}

	int id, edgeNum;
	vector<node>::iterator ivNode;
	for(i = 0, ivNode = vNode.begin(); i < lineNum; i++, ivNode++)
	{
		infile >> id >> edgeNum;
		for(j = 0; j < edgeNum; j++)
		{
			edgeInfo ei;
			ei.u = id;
			infile >> ei.v >> ei.t1 >> ei.t2;
			vEdgeStream.push_back(ei);
		}
	}

	infile.close();

	sort(vEdgeStream.begin(), vEdgeStream.end(), eCompare);
/*	vector<edgeInfo>::iterator ivEdgeStream;
	for(ivEdgeStream = vEdgeStream.begin(); ivEdgeStream != vEdgeStream.end(); ivEdgeStream++)
	{
		cout << (*ivEdgeStream).u << "\t"  << (*ivEdgeStream).v << "\t" << (*ivEdgeStream).t1 << "\t" << (*ivEdgeStream).t2 << endl;
	}*/
}

void esGraph::earliestArrivalTime(int u, int ts, int te, vector<int> &vTime)
{
	vector<int>::iterator ivTime;
	int i;
	for(i = 0; i < vNode.size(); i++)
	{
		if(i == u)
			vTime.push_back(0);
		else
			vTime.push_back(INFINITE);
	}

	vector<edgeInfo>::iterator ivEdgeStream;
	for(ivEdgeStream = vEdgeStream.begin(); ivEdgeStream != vEdgeStream.end(); ivEdgeStream++)
	{
		if((*ivEdgeStream).t2 >= ts && ts >= vTime[(*ivEdgeStream).u])
		{
			if((*ivEdgeStream).t2 < vTime[(*ivEdgeStream).v])
				vTime[(*ivEdgeStream).v] = (*ivEdgeStream).t2;
		}
		else if((*ivEdgeStream).t1 > te)
			break;
	}

	for(ivTime = vTime.begin(), i = 0; ivTime != vTime.end(); ivTime++, i++)
	{
		if(*ivTime != INFINITE)
			cout << i << "\t" << *ivTime << endl;
	}
}

int esGraph::nodePos(int id, int t, int &x, int &y)
{
	vector<nodeInfo>::iterator ivnodeInfo;
	vector<nodeInfo>::iterator ivnodeInfo2;
	for(ivnodeInfo = vNode[id].vNodeInfo.begin(); ivnodeInfo != vNode[id].vNodeInfo.end(); ivnodeInfo++)
	{
		if((*ivnodeInfo).t1 <= t && (*ivnodeInfo).t2 >= t && ivnodeInfo + 1 != vNode[id].vNodeInfo.end())
		{
			ivnodeInfo2 = ivnodeInfo + 1;
			x = (*ivnodeInfo).x + ((*ivnodeInfo2).x - (*ivnodeInfo).x) * (t- (*ivnodeInfo).t1) / ((*ivnodeInfo).t2 - (*ivnodeInfo).t1);
			y = (*ivnodeInfo).y + ((*ivnodeInfo2).y - (*ivnodeInfo).y) * (t- (*ivnodeInfo).t1) / ((*ivnodeInfo).t2 - (*ivnodeInfo).t1);
			return 1;
		}
	}
	
	return 0;
}

int esGraph::nodeDist(int id1, int id2, int t, int &dist)
{
	int x1, x2, y1, y2;
	if(nodePos(id1, t, x1, y1) && nodePos(id2, t, x2, y2))
	{
		dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
		return 1;
	}
	return 0;
}
/*
bool esGraph::spatialDirectReachable(int id1, int id2, int esp, int t1, int t2, vector<pair<int, int> > &vpTime)
{
	int i, dist;
	int t = t2 - t1;
	int ts = -1;
	int te;
	for(i = 0; i < t; i++)
	{
		if(nodeDist(id1, id2, t1 + i, dist))
		{
			if(dist <= esp)
			{
				if(ts == -1)
					ts = t1 + i;
				if(i + 1 == t)
				{
					te = t1 + i;
					vpTime.push_back(make_pair(ts, te));
				}
			}
			else if(dist > esp && ts != -1)
			{
				te = t1 + i;
				vpTime.push_back(make_pair(ts, te));
				ts = -1;
			}
		}
	}

	if(!vpTime.empty())	
		return true;
	else
		return false;
}
*/
bool esGraph::timeRangeDistReachable(int id1, int id2, int t1, int t2, int esp)
{
	int i, dist;
	int t = t2 - t1;
	for(i = 0; i < t; i++)
	{
		if(nodeDist(id1, id2, t1 + i, dist))
			if (dist <= esp)
				return true;
	}
	return false;
}

void esGraph::intersect(vector<pair<int, int> > vp, int t1, int t2, vector<pair<int, int> > &rvp)
{
	vector<pair<int, int> >::iterator ivp;
	for(ivp = vp.begin(); ivp != vp.end(); ivp++)
	{
		if(t1 <= (*ivp).first && t2>= (*ivp).first && t2 <= (*ivp).second)
			rvp.push_back(make_pair((*ivp).first, t2));
		else if(t1 >= (*ivp).first && t1 <= (*ivp).second && t2 >= (*ivp).second)
			rvp.push_back(make_pair(t1, (*ivp).second));
		else if(t1 >= (*ivp).first && t2 <= (*ivp).second)
			rvp.push_back(make_pair(t1, t2));
		else if(t1 <= (*ivp).first && t2 >= (*ivp).second)
			rvp.push_back(make_pair((*ivp).first, (*ivp).second));
	}
}
	
void esGraph::SSSTReachable(int u, int esp, int ts, int te, vector<int> &vTime)
{
	vector<int>::iterator ivTime;
	int i;
	for(i = 0; i < vNode.size(); i++)
	{
		if(i == u)
			vTime.push_back(0);
		else
			vTime.push_back(INFINITE);
	}

	vector<edgeInfo>::iterator ivEdgeStream;
	for(ivEdgeStream = vEdgeStream.begin(); ivEdgeStream != vEdgeStream.end(); ivEdgeStream++)
	{
		cout << "inline:\t" << (*ivEdgeStream).u << "\t" << (*ivEdgeStream).v << "\t" << (*ivEdgeStream).t1 << "\t" << (*ivEdgeStream).t2 << endl;
		if((*ivEdgeStream).t2 >= ts && te >= vTime[(*ivEdgeStream).u])
		{
			vector<pair<int, int> > vps, vpTime;
			int t;
			if((*ivEdgeStream).t1 > ts)
				t = (*ivEdgeStream).t1;
			else
				t = ts;
				cout << "Temporal\t" << (*ivEdgeStream).u << "\t" << (*ivEdgeStream).v << "\t" << (*ivEdgeStream).t1 << "\t" << (*ivEdgeStream).t2 << "\t" << vTime[(*ivEdgeStream).u] << "\t" << vTime[(*ivEdgeStream).v] << endl ;
			if(spatialDirectReachable((*ivEdgeStream).u, (*ivEdgeStream).v, esp, t, te, vps))
			//if((*ivEdgeStream).t2 < vTime[(*ivEdgeStream).v])
//				if((*ivEdgeStream).t1 < vTime[(*ivEdgeStream).v])
			{
				intersect(vps, t, te, vpTime);
				cout << (vpTime[0].first) << "\t" << vTime[(*ivEdgeStream).v] << endl; 
				if(vpTime[0].first < vTime[(*ivEdgeStream).v])
				{
//				if(timeRangeDistReachable(u, (*ivEdgeStream).v, ts, te, esp))
//					vTime[(*ivEdgeStream).v] = (*ivEdgeStream).t2;
					cout << (*ivEdgeStream).u << "\t" << (*ivEdgeStream).v << "\t" << (*ivEdgeStream).t1 << "\t" << (*ivEdgeStream).t2 << endl;
					vTime[(*ivEdgeStream).v] = vpTime[0].first;
				}
			}
			else
				cout << "not spatial reachability" << endl;
		}
		else if((*ivEdgeStream).t1 > te)
		{
			cout << "Out:" << (*ivEdgeStream).t1 << endl;
			break;
		}
	}

	for(ivTime = vTime.begin(), i = 0; ivTime != vTime.end(); ivTime++, i++)
	{
		if(*ivTime != INFINITE)
			cout << i << "\t" << *ivTime << endl;
	}
}

bool esGraph::spatialDirectReachable(int id1, int id2, int esp, int t1, int t2, vector<pair<int, int> > &vpTime)
{
	int i, dist;
	int t = t2 - t1;
	int ts = -1;
	int te;
	for(i = 0; i < t; i++)
	{
		if(nodeDist(id1, id2, t1 + i, dist))
		{
			//	cout << dist << endl;
			if(dist <= esp)
			{
				if(ts == -1)
					ts = t1 + i;
				if(i + 1 == t)
				{
					te = t1 + i;
					vpTime.push_back(make_pair(ts, te));
				}
				//				cout << dist << endl;
			}
			else if(dist > esp && ts != -1)
			{
				te = t1 + i;
				vpTime.push_back(make_pair(ts, te));
				ts = -1;
			}
			
		}
	}

	if(!vpTime.empty())	
		return true;
	else
		return false;
}
