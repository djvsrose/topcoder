#ifndef DISK_SCHEDULER_H_INCLUDED
#define DISK_SCHEDULER_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <algorithm>
#include <utility>
using namespace std;


class DiskScheduler
{
	struct Info
	{
		int dist;
		int fromId, toId;
	};

	struct ShortestPathCompare
	{
		ShortestPathCompare(const vector<int>& sectors)
			: sectors_(sectors) {}

		bool operator() (const Info& lhs, const Info& rhs)
		{
			return CalcShortestPath(lhs) < CalcShortestPath(rhs);
		}

		inline int GetDist(int toId)
		{ return min(sectors_[toId], 100 - sectors_[toId]); }

		int CalcShortestPath(const Info& info)
		{
			// the shortest path for taking only one U turn should be either 
			// either (100 + GetDist(info.fromId) - dist) --- take U turn at fromId
			// or     (100 + GetDist(info.toId) - dist) -- take U turn at toId
			int dist1 = 100 + GetDist(info.fromId) - info.dist;
			int dist2 = 100 + GetDist(info.toId) - info.dist;
			return min(dist1, dist2);
		}

		const vector<int>& sectors_;
	};

public:
	int optimize(int start, vector<int> sectors)
	{
		int numSectors = sectors.size();
		int idLastSector = numSectors - 1;

		// transform the sectors such that all sectors are related to start
		for (int i = 0; i < numSectors; ++i) {
			sectors[i] = (sectors[i] - start + 100) % 100;
		}

		sort(sectors.begin(), sectors.end());
		
		// dists stores the distance between each sector
		Info info;
		vector<Info> distInfos;
		info.fromId = idLastSector;
		info.toId = 0;
		info.dist = 100 + sectors[0] - sectors[idLastSector];
		distInfos.push_back( info );
		for (int i=1; i<numSectors; ++i) {
			info.dist = sectors[i] - sectors[i-1];
			info.fromId = i - 1;
			info.toId = i;
			distInfos.push_back(info);
		}
	
		vector<Info>::iterator it = 
			min_element(distInfos.begin(), distInfos.end(), ShortestPathCompare(sectors));

		int ret[4];
		// ret[0], ret[1] does not take any U turn
		ret[0] = sectors[idLastSector];
		ret[1] = 100 - sectors[0];

		// dist[2], dist[3] take one U turn
		ret[2] = sectors[it->fromId] + (100 - it->dist);
		ret[3] = (100 - sectors[it->toId]) + (100 - it->dist);

		return *min_element(ret, ret+4);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
#endif