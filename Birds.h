#ifndef BIRDS_H_INCLUDED
#define BIRDS_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

int GetDuration(int fromMonth, int fromDate, int toMonth, int toDate)
{
	const int MonthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (fromMonth == toMonth)
		return toDate - fromDate;

	int durations = MonthDays[fromMonth] - fromDate + 1;
	for (int i = fromMonth + 1; i < toMonth; ++i) {
		durations += MonthDays[i];
	}
	durations += toDate;
	return durations - 1;
}

double GetDistance(int fromX, int fromY, int toX, int toY)
{
	int deltaX = fromX - toX;
	int deltaY = fromY - toY;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

class Birds
{
	struct BirdStop
	{
		int mon, date;
		int x, y;
		int duration;

		bool operator< (const BirdStop& other) const
		{
			return mon < other.mon || (mon == other.mon && date < other.date);
		}
	};

	struct Region
	{
		Region() : duration(0) {}

		Region(const BirdStop& stop) 
			: duration(0)
		{
			AddStop(stop);
		}

		void AddStop(const BirdStop& stop)
		{
			stops.push_back(stop);
			duration += stop.duration;
		}

		vector<BirdStop> stops;
		int duration;
	};

	vector<BirdStop> CombineRedundantStops(const vector<BirdStop>& stops)
	{
		vector<bool> IsRedundantStop;

		IsRedundantStop.push_back(false);	// the first stop can't be redundant
		for (unsigned int i = 1; i < stops.size(); ++i) {
			IsRedundantStop.push_back( stops[i].x == stops[i-1].x && stops[i].y == stops[i-1].y );
		}

		vector<BirdStop> ret;
		for (unsigned int i = 0; i < IsRedundantStop.size(); ++i) {
			if (!IsRedundantStop[i])
				ret.push_back( stops[i] );
		}

		return ret;
	}

	void CalculateDurations(vector<BirdStop>& stops)
	{
		const unsigned int numStops = stops.size();
		int numDays = 0;
		for (unsigned int i = 1; i < numStops; ++i) {
			int dur = GetDuration(stops[i-1].mon, stops[i-1].date, stops[i].mon, stops[i].date);
			stops[i-1].duration = dur;
			numDays += dur;
		}
//		stops.back().duration = 365 - numDays;
		stops.back().duration = GetDuration(stops.back().mon, stops.back().date, 12, 31) + 1;	// Dec 31 has to be included, hence + 1
	}


	bool CanCombine(const Region& region, const BirdStop& stop) const
	{
		// criteria for combine a BirdStop into a region
		// stop.duration < 90	=> if it is greater equal to 90, there is no need to combine
		// region.duration < 90 => if it is greater equal to 90, there is no need to combine more
		// distance between stop and all stops inside the region should be less than 1000
		if (region.duration >= 90 || stop.duration >= 90)
			return false;

		for (unsigned int i = 0; i < region.stops.size(); ++i) {
			if (GetDistance(region.stops[i].x, region.stops[i].y, stop.x, stop.y) >= 1000)
				return false;
		}

		return true;
	}

	bool IsRegionApart(const Region& reg1, const Region& reg2)
	{
		for (unsigned int i = 0; i < reg1.stops.size(); ++i) {
			for (unsigned int j = 0; j < reg2.stops.size(); ++j) {
				double dist = GetDistance(reg1.stops[i].x, reg1.stops[i].y, reg2.stops[j].x, reg2.stops[j].y);
				if (dist < 1000.f)
					return false;
			}
		}

		return true;
	}

	BirdStop Parse(const string& input)
	{
		BirdStop stop;
		istringstream ss(input);
		string token;
		getline(ss, token, ',');	
		stop.x = atoi(token.c_str());
		getline(ss, token, ',');
		stop.y = atoi(token.c_str());
		getline(ss, token, ',');
		stop.mon = atoi(token.c_str());
		getline(ss, token, ',');
		stop.date = atoi(token.c_str());
		return stop;
	}

public:

	int isMigratory(vector<string> birdMoves)
	{
		vector<BirdStop> stops;

		for (unsigned int i = 0; i < birdMoves.size(); ++i) {
			stops.push_back( Parse(birdMoves[i]) );
		}

		sort(stops.begin(), stops.end());

		// combine stops that are adjacent to each other and have exact same location
		stops = CombineRedundantStops(stops);

		// calculate durations
		CalculateDurations(stops);

		// calcuate distance between Regions
		// there are different ways to form region, so we try all kinds of possibility
		for (unsigned int i = 0; i < stops.size()-1; ) {
			// form the first region
			Region region1(stops[i]);
			int ii = i + 1;
			while (CanCombine(region1, stops[ii])) {
				region1.AddStop(stops[ii++]);
			}
		
			if (region1.duration < 90) {		// if the duration is still less than 90, then we can skip
				i = ii;
				continue;
			} 
			else {
				i++;							// normal path to increase i
			}
				
			for (unsigned int j = ii; j < stops.size(); ) {
				Region region2(stops[j]);
				unsigned int jj = j + 1;
				while (jj < stops.size() && CanCombine(region2, stops[jj])) {
					region2.AddStop(stops[jj++]);
				}

				if (region2.duration < 90) {
					j = jj;
					continue;
				}
				else {
					j++;						// normal path to increase j
				}

				if (IsRegionApart(region1, region2))
					return 1;
			}
		}

		return 0;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////
#endif