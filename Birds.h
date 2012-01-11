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

class Birds
{
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

	struct BirdStop
	{
		BirdStop() : redundant(false) {}

		int mon, date;
		int x, y;
		bool redundant;

		bool operator< (const BirdStop& other) const
		{
			return mon < other.mon || (mon == other.mon && date < other.date);
		}
	};

	struct Region
	{
		Region(const BirdStop& stop) 
		{
			stops.push_back(stop);
		}

		vector<BirdStop> stops;
	};

	bool IsRegionApart(const Region& reg1, const Region& reg2)
	{
		for (int i = 0; i < reg1.stops.size(); ++i) {
			for (int j = 0; j < reg2.stops.size(); ++j) {
				if (GetDistance(reg1.stops[i].x, reg1.stops[i].y, reg2.stops[j].x, reg2.stops[j].y) < 1000)
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

		for (int i = 0; i < birdMoves.size(); ++i) {
			stops.push_back( Parse(birdMoves[i]) );
		}

		sort(stops.begin(), stops.end());

		// group stops into regions, if two location are less than 1000 miles away from each other
		// according to the question, it is in the same region
		for (int i = 1; i < stops.size(); ++i) {
			if (GetDistance(stops[i-1].x, stops[i-1].y, stops[i].x, stops[i].y) < 1000) {
				// mark redundant stops
				stops[i].redundant = true;
			}
		}

		vector<Region> regions;
		for (int i = 0; i < stops.size(); ++i) {
			if (stops[i].redundant == false)
				regions.push_back(stops[i]);
			else if (regions.back().stops.back().x != stops[i].x && regions.back().stops.back().y != stops[i].y)
				regions.back().stops.push_back(stops[i]);
		}

		// calculate durations
		const int numRegions = regions.size();
		int numDays = 0;
		vector<int> durations;
		for (int i = 1; i < numRegions; ++i) {
			int dur = GetDuration(regions[i-1].stops[0].mon, regions[i-1].stops[0].date, regions[i].stops[0].mon, regions[i].stops[0].date);
			durations.push_back( dur );
			numDays += dur;
		}
		durations.push_back( 365 - numDays );

		// check duration, we need to stay in 2 places for more than 90 days
		vector<int> longStays;
		for (int i = 0; i < durations.size(); ++i) {
			 if (durations[i] >= 90) 
				 longStays.push_back(i);
		}
		if (longStays.size() < 2)
			return 0;

		// calcuate distance between longStays
		for (int i = 0; i < longStays.size(); ++i) {
			for (int j = i+1; j < longStays.size(); ++j) {
				if (IsRegionApart(regions[i], regions[j]))
					return 1;
			}
		}

		return 0;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////
#endif