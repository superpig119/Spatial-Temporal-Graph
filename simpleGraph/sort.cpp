#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

using namespace std;

bool pCompare(pair<int, int> p1, pair<int, int> p2)
{
	return (p1.first < p2.first);
}


int main()
{
	vector<pair<int, int> > vp;
	vector<pair<int, int> >::iterator ivp;
	vp.push_back(make_pair(1,5));
	vp.push_back(make_pair(3,6));
	vp.push_back(make_pair(2,3));
	vp.push_back(make_pair(4,7));

	sort(vp.begin(), vp.end(), pCompare);
	for(ivp = vp.begin(); ivp != vp.end(); ivp++)
		cout << (*ivp).first << "\t" << (*ivp).second << endl;
}
