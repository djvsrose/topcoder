#ifndef INTERVALS_H_INCLUDED
#define INTERVALS_H_INCLUDED
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <list>
#include <map>
#include <sstream>
#include <cassert>

using namespace std;

class Intervals
{
	typedef pair<int,int> Interval;

	struct Node {
		Node() : open_(0), close_(0), v_(INT_MIN) {}

		int open_, close_;
		int v_;
	};

	string createPairString(int open, int close) {
		string ret;
		char buf[80];
		sprintf(buf, "%d", open);
		ret += buf;
//		ret += itoa(open, buf, 10);
		ret += ":";
		sprintf(buf, "%d", close);
		ret += buf;
//		ret += itoa(close, buf, 10);
		return ret;
	}

public:
	vector<string> partition(vector<string> intervals)
	{
		vector<string> result;
		map<int, Node> nodeMap;

		vector<string>::iterator iend = intervals.end();
		for (vector<string>::iterator it = intervals.begin(); it != iend; ++it) {
			Interval interval;
			// first convert "a:b" to Interval
			interval.first = atoi(it->substr(0, it->find(':')).c_str());
			interval.second = atoi(it->substr(it->find(':') + 1).c_str());
			nodeMap[interval.first].open_ += 1;
			nodeMap[interval.second].close_ += 1;
		}

		vector<Node> nodes;
		for (map<int,Node>::iterator it = nodeMap.begin(); it != nodeMap.end(); ++it) {
			nodes.push_back(it->second);
			nodes.back().v_ = it->first;
		}

		vector<Node>::iterator nend = nodes.end();
		int opening = INT_MIN;
		int open_close_count = 0;
		for (vector<Node>::iterator it = nodes.begin(); it != nend; ++it) {
			if (it->close_ && it->open_) {
				if (opening != INT_MIN && (it->v_ > opening)) {	// close the opening
					result.push_back(createPairString(opening, it->v_ - 1));
				}
				result.push_back(createPairString(it->v_, it->v_));
				open_close_count += (it->open_ - it->close_);
				opening = open_close_count > 0 ? it->v_ + 1 : INT_MIN;
			}
			else if (it->close_) {
				result.push_back(createPairString(opening, it->v_));
				open_close_count += (it->open_ - it->close_);
				opening = open_close_count == 0 ? INT_MIN : it->v_ + 1;
			}
			else if (it->open_) {
				if (opening != INT_MIN && it->v_ > opening) {
					result.push_back(createPairString(opening, it->v_ - 1));
				}
				opening = it->v_;
				open_close_count += (it->open_ - it->close_);
			}
			else {
				assert(false);
			}
		}

		return result;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////
#endif