#include "graph.h"

bool pCompare(pair<int, int> p1, pair<int, int> p2)
{
	return (p1.first < p2.first);
}

void Graph::buildGraph(const char* fileName)
{
	ifstream infile(fileName);
	int lineNum;
	infile >> lineNum;
	int i, j;
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
			n.vnodeInfo.push_back(ni);
		}
		vnode.push_back(n);
	}

	int id, edgeNum;
	vector<node>::iterator ivnode;
	for(i = 0, ivnode = vnode.begin(); i < lineNum; i++, ivnode++)
	{
		infile >> id >> edgeNum;
		for(j = 0; j < edgeNum; j++)
		{
			edgeInfo ei;
			infile >> ei.id >> ei.t1 >> ei.t2;
			(*ivnode).ledgeInfo.push_back(ei);
		}
	}

	infile.close();
}

int Graph::nodeNum()
{
	return vnode.size();
}

int Graph::edgeNum()
{
	vector<node>::iterator ivnode;
	int en = 0;
	for(ivnode = vnode.begin(); ivnode != vnode.end(); ivnode++)
	{
		en += (*ivnode).ledgeInfo.size();
	}
	return en;
}
    
void Graph::showNeighbors(int id)
{
    if(id >= 0 && id < vnode.size())
    {
        list<edgeInfo>::iterator iledgeInfo;
        cout << id << "'s neighbors:" << endl;
        for(iledgeInfo = vnode[id].ledgeInfo.begin(); iledgeInfo != vnode[id].ledgeInfo.end(); iledgeInfo++)
        {
            cout << (*iledgeInfo).id << "\t" << (*iledgeInfo).t1 << "\t" << (*iledgeInfo).t2 << endl;
        }
    }
}
	
int Graph::nodePos(int id, int t, int &x, int &y)
{
	vector<nodeInfo>::iterator ivnodeInfo;
	vector<nodeInfo>::iterator ivnodeInfo2;
	for(ivnodeInfo = vnode[id].vnodeInfo.begin(); ivnodeInfo != vnode[id].vnodeInfo.end(); ivnodeInfo++)
	{
		if((*ivnodeInfo).t1 <= t && (*ivnodeInfo).t2 >= t && ivnodeInfo + 1 != vnode[id].vnodeInfo.end())
		{
			ivnodeInfo2 = ivnodeInfo + 1;
			x = (*ivnodeInfo).x + ((*ivnodeInfo2).x - (*ivnodeInfo).x) * (t- (*ivnodeInfo).t1) / ((*ivnodeInfo).t2 - (*ivnodeInfo).t1);
			y = (*ivnodeInfo).y + ((*ivnodeInfo2).y - (*ivnodeInfo).y) * (t- (*ivnodeInfo).t1) / ((*ivnodeInfo).t2 - (*ivnodeInfo).t1);
			return 1;
		}
	}
	return 0;
}
	
int Graph::nodeDist(int id1, int id2, int t, int &dist)
{
	int x1, x2, y1, y2;
	if(nodePos(id1, t, x1, y1) && nodePos(id2, t, x2, y2))
	{
		dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
		return 1;
	}
	return 0;
}
	
int Graph::nodeDistRange(int id1, int id2, int t1, int t2, vector<int> &vDistRange)
{
	int i, dist;
	int t = t2 - t1;
	for(i = 0; i < t; i++)
	{
		if(nodeDist(id1, id2, t1 + i, dist))
			vDistRange.push_back(dist);
		else
			return t1 + i;
	}
	return t2 - t1;
}

	
bool Graph::spatialDirectReachable(int id1, int id2, int esp, int t1, int t2, vector<pair<int, int> > &vpTime)
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
		
bool Graph::edgeExist(int id1, int id2, int t)
{
	list<edgeInfo>::iterator iledgeInfo;
	for(iledgeInfo = vnode[id1].ledgeInfo.begin(); iledgeInfo != vnode[id1].ledgeInfo.end(); iledgeInfo++)
	{
		if(id2 == (*iledgeInfo).id && t >= (*iledgeInfo).t1 && t <= (*iledgeInfo).t2)
			return true;
	}
	
/*	for(iledgeInfo = vnode[id2].ledgeInfo.begin(); iledgeInfo != vnode[id2].ledgeInfo.end(); iledgeInfo++)
	{
		if(id1 == (*iledgeInfo).id && t >= (*iledgeInfo).t1 && t <= (*iledgeInfo).t2)
			return true;
	}*/

	return false;
}
	
int Graph::edgeExistTime(int id1, int id2, vector<pair<int, int> > &vpTime)
{
	list<edgeInfo>::iterator iledgeInfo;
	for(iledgeInfo = vnode[id1].ledgeInfo.begin(); iledgeInfo != vnode[id1].ledgeInfo.end(); iledgeInfo++)
	{
		if(id2 == (*iledgeInfo).id)
			vpTime.push_back(make_pair((*iledgeInfo).t1, (*iledgeInfo).t2));
	}

/*	for(iledgeInfo = vnode[id2].ledgeInfo.begin(); iledgeInfo != vnode[id2].ledgeInfo.end(); iledgeInfo++)
	{
		if(id1 == (*iledgeInfo).id)
			vpTime.push_back(make_pair((*iledgeInfo).t1, (*iledgeInfo).t2));
	}*/		//处理无向图的时候
	
	sort(vpTime.begin(), vpTime.end(), pCompare);

/*	vector<pair<int, int> >::iterator ivpTime, ivpTime2;
	for(ivpTime = vpTime.begin(); ivpTime != vpTime.end(); ivpTime++)
	{
		if(ivpTime + 1 != vpTime.end())
		{
			ivpTime2 = ivpTime + 1;
			if((*ivpTime2).first <= (*ivpTime).second)
			{
				(*ivpTime).second = (*ivpTime2).second;
				vpTime.erase(ivpTime2);
			}
		}
		
	}*/

	return vpTime.size();
}

	
void Graph::intersect(vector<pair<int, int> > vp, int t1, int t2, vector<pair<int, int> > &rvp)
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

bool Graph::temporalDirectReachable(int id1, int id2, int t1, int t2, vector<pair<int, int> > &vpTime)
{
	vector<pair<int, int> > vp;
	if(edgeExistTime(id1, id2, vp))
	{
		intersect(vp, t1, t2, vpTime);
		if(!vpTime.empty())
			return true;
		else
			return false;
	}
	else
		return false;
}
	
bool Graph::STDReachable(int id1, int id2, int esp, int t1, int t2, vector<pair<int, int> > &vpTime)
{
	vector<pair<int, int> > vps, vpt;
	vector<pair<int, int> >::iterator ivp;
	if(spatialDirectReachable(id1, id2, esp, t1, t2, vps) && temporalDirectReachable(id1, id2, t1, t2, vpt))
	{
	/*	cout << "vps:" << endl;
		for(ivp = vps.begin(); ivp != vps.end(); ivp++)
		{
			cout << (*ivp).first << "\t" << (*ivp).second << endl;
		}
		cout << "vpt:" << endl;
		for(ivp = vpt.begin(); ivp != vpt.end(); ivp++)
		{
			cout << (*ivp).first << "\t" << (*ivp).second << endl;
		}*/
		for(ivp = vpt.begin(); ivp != vpt.end(); ivp++)
		{
			intersect(vps, (*ivp).first, (*ivp).second, vpTime);
		}
		
		if(vpTime.size())
			return true;
	}
	return false;
}
	
bool Graph::STReachable(int id1, int id2, int esp, int t1, int t2, vector<pathInfo> vPathInfo)
{
	vector<node>::iterator ivnode;
	list<edgeInfo>::iterator iledgeInfo;
	queue<pathInfo> qPathInfo;
	pathInfo p;
	p.id = id1;
	p.t1 = t1;
	p.t2 = t2;
	qPathInfo.push(p);
	map<int, bool> mnode;
	for(ivnode = vnode.begin(); ivnode != vnode.end(); ivnode++)
	{
		mnode[(*ivnode).id] = false;
	}
	while(!qPathInfo.empty())
	{
		pathInfo p1 = qPathInfo.front();
		qPathInfo.pop();
		mnode[p1.id] = true;
		for(iledgeInfo = vnode[p1.id].ledgeInfo.begin(); iledgeInfo != vnode[p1.id].ledgeInfo.end(); iledgeInfo++)
		{
			vector<pair<int, int> > vpTime;
				
		/*	if(mnode[(*iledgeInfo).id])
				continue;*/
			if(STDReachable(p1.id, (*iledgeInfo).id, esp, p1.t1, t2, vpTime))
			{ 
				int dist;
				nodeDist(p1.id, (*iledgeInfo).id, (*(vpTime.begin())).first, dist);
				cout << "p1:" << p1.id << "\tp2:" << (*iledgeInfo).id << "\t" << (*(vpTime.begin())).first << "\t" << (*(vpTime.begin())).second << "\t" << dist << endl;
				if((*iledgeInfo).id == id2)
					return true;
				pathInfo pp;
				pp.id = (*iledgeInfo).id;
				pp.t1 = (*(vpTime.begin())).first;
				pp.t2 = (*(vpTime.begin())).second;
				if(!mnode[(*iledgeInfo).id])
				{
					qPathInfo.push(pp);
					mnode[(*iledgeInfo).id] = true;
				}
			}
		}
	}
	return false;
}
	
void Graph::SSSTReachable(int u, int esp, int t1, int t2, vector<int> &vTime)
{
	vector<int>::iterator ivTime;
	int i;
	for(i = 0; i < vnode.size(); i++)
	{
		if(i == u)
			vTime.push_back(0);
		else
			vTime.push_back(INFINITE);
	}

	vector<node>::iterator ivnode;
	list<edgeInfo>::iterator iledgeInfo;
	queue<pathInfo> qPathInfo;
	pathInfo p;
	p.t1 = t1;
	p.t2 = t2;
	p.id = u;
	qPathInfo.push(p);
	map<int, bool> mnode;
	for(ivnode = vnode.begin(); ivnode != vnode.end(); ivnode++)
	{
		mnode[(*ivnode).id] = false;
	}
	mnode[p.id] = true;

	while(!qPathInfo.empty())
	{
		pathInfo p1 = qPathInfo.front();
		mnode[p1.id] = true;
		qPathInfo.pop();
		for(iledgeInfo = vnode[p1.id].ledgeInfo.begin(); iledgeInfo != vnode[p1.id].ledgeInfo.end(); iledgeInfo++)
		{
			if((*iledgeInfo).t2 >= p1.t1 && (*iledgeInfo).t1 < p1.t2)
			{
				vector<pair<int, int> > vpTime;

				if(STDReachable(p1.id, (*iledgeInfo).id, esp, max(p1.t1, (*iledgeInfo).t1), min(p1.t2, (*iledgeInfo).t2), vpTime))
				{ 
					if((*(vpTime).begin()).first < vTime[(*iledgeInfo).id])
					{
						int dist;
						nodeDist(p1.id, (*iledgeInfo).id, (*(vpTime.begin())).first, dist);
						cout << "p1:" << p1.id << "\tp2:" << (*iledgeInfo).id << "\t" << (*(vpTime.begin())).first << "\t" << (*(vpTime.begin())).second << "\t" << dist << endl;
			//			if((*iledgeInfo).id == id2)
				/*		pathInfo pp;
						pp.id = (*iledgeInfo).id;
						pp.t1 = (*(vpTime.begin())).first;
						pp.t2 = (*(vpTime.begin())).second;*/
						if(!mnode[(*iledgeInfo).id])
						{
							pathInfo p2;
							p2.id = (*iledgeInfo).id;
							if((*(vpTime.begin())).first > p1.t1)
							{
								p2.t1 = (*(vpTime.begin())).first;
								vTime[(*iledgeInfo).id] = vpTime[0].first;
							}
							else
							{
								p2.t1 = p1.t1;
								vTime[(*iledgeInfo).id] = p1.t1;
							}
						//	cout << p2.id << "\t" << vpTime[0].first << "\t" << vpTime[0].second << endl;
							p2.t2 = p1.t2;
							qPathInfo.push(p2);
							mnode[(*iledgeInfo).id] = true;
						}
					}
				}
			}
		}
	}
}
